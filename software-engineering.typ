#set text(font: "New Computer Modern", lang: "en", weight: "light", size: 11pt)
#set page(margin: 1.75in)
#set par(leading: 0.55em, spacing: 0.85em, first-line-indent: 1.8em, justify: true)
#set heading(numbering: "1.1")
#set math.equation(numbering: "(1)")

#show figure.caption: set align(center)
#show heading: set block(above: 1.4em, below: 1em)
#show outline.entry.where(level: 1): it => { show repeat : none; v(1.1em, weak: true); text(size: 1em, strong(it)) }
#show raw.where(block: true): block.with(inset: 1em, width: 100%, fill: luma(254), stroke: (left: 5pt + luma(245), rest: 1pt + luma(245)))
#show figure.where(kind: raw): it => { set align(left); it }
#show raw: set text(font:"CaskaydiaCove NFM", lang: "en", weight: "light", size: 9pt)
#show sym.emptyset : sym.diameter 

#let reft(reft) = box(width: 8pt, place(dy: -8pt, 
  box(radius: 100%, width: 9pt, height: 9pt, inset: 1pt, stroke: .5pt, // fill: black,
    align(center + horizon, text(font: "CaskaydiaCove NFM", size: 7pt, repr(reft)))
  )
))

#show raw: r => { show regex("t(\d)"): it => { reft(int(repr(it).slice(2, -1))) }; r }

#let note(body) = block(inset: 1em, stroke: (paint: silver, dash: "dashed"), body)
// #let note(body) = block(inset: 1em, fill: luma(254), stroke: (thickness: 1pt, paint: luma(245)), body)

#page(align(center + horizon, {
  heading(outlined: false, numbering: none, text(size: 1.5em)[Software Engineering]) 
  text(size: 1.3em)[Cicio Ionuț]
  align(bottom, datetime.today().display("[day]/[month]/[year]"))
}))

#page(outline(indent: auto, depth: 3))

#set page(numbering: "1")

= Software models

Software projects require *design choices* that often can't be driven by experience or reasoning alone. That's why a *model* of the project is needed to compare different solutions.

== The _"Amazon Prime Video"_ article 

If you were tasked with designing the software architecture for Amazon Prime Video, which choices would you make? What if you had the to keep the costs minimal? Would you use a distributed architecture or a monolith application?

More often than not, monolith applications are considered more costly and less scalable than the counterpart, due to an inefficient usage of resources. But, in a recent article, a Senior SDE at Prime Video describes how they _"*reduced the cost* of the audio/video monitoring infrastructure by *90%*"_ @prime by using a monolith architecture.

There isn't a definitive way to answer these type of questions, but one way to go about it is building a model of the system to compare the solutions. In the case of Prime Video, _"the audio/video monitoring service consists of three major components:"_ @prime
- the _media converter_ converts input audio/video streams 
- the _defect detectors_ analyze frames and audio buffers in real-time
- the _orchestrator_ controls the flow in the service

#figure(caption: "audio/video monitoring system")[#{
  set text(font: "CaskaydiaCove NFM", weight: "light", lang: "en")
  image("public/audio-video-monitor.svg", width: 90%)
}] <audio-video-monitor>

To derive conclusions on this system, its components can be modeled as *Markov decision processes*.

== Models // Formal theory <traffic>

=== Markov chains <markov-chain>

A Markov chain $M$ is described by a set of *states* $S$ and the *transition probability* $p : S times S -> [0, 1]$ such that $p(s'|s)$ is the probability to transition to state $s'$ if the current state is $s$. The transition probability $p$ is constrained by @markov-chain-constrain

$ forall s in S space.en sum_(s' in S) p(s'|s) = 1 $ <markov-chain-constrain>

For example, the weather can be modeled with $S = { "sunny", "rainy" }$ and $p$ such that

#grid(
  columns: (auto, auto),
  align: center + horizon,
  gutter: 1em,
  math.equation[
    p = 
    #table(
      columns: (auto, auto, auto),
      stroke: .1pt,
      align: center + horizon,
      table.header([], [sunny], [rainy]),
      [sunny], [0.8], [0.2],
      [rainy], [0.5], [0.5]
    )
  ],
  {
    set text(font: "CaskaydiaCove NF", weight: "light", lang: "en")
    image("public/weather-system.svg")
  }
)

If a Markov chain $M$ transitions at discrete time steps, i.e. the time steps $t_0, t_1, t_2, ...$ are a *countable*, then it's called a DTMC (discrete-time Markov chain), otherwise it's called a CTMC (continuous-time Markov chain).

=== Markov decision processes <mdp>

A Markov decision process (MDP), despite sharing the name, is *different* from a Markov chain, because transitions are influenced by an external environment. A MDP $M$ is a tuple $(U, X, Y, p, g)$ s.t.
- $U$ is the set of *input values*
- $X$ is the set of *states*
- $Y$ is the set of *output values* 
- $p : X times X times U -> [0, 1]$ is such that $p(x'|x, u)$ is the probability to *transition* from state $x$ to state $x'$ when the *input value* is $u$
- $g : X -> Y$ is the *output function*
- $x_0 in X$ is the *initial state*

The same constrain in @markov-chain-constrain holds for MDPs, with an important difference: *for each input value*, the sum of the transition probabilities for *that input value* must be 1.

$ forall x in X space.en forall u in U space.en sum_(x' in X) p(x'|x, u) = 1 $

==== Example <mdp-example>

The development process of a company can be modeled as a MDP \ $M = (U, X, Y, p, g)$ s.t.
- $U = {epsilon}$ #footnote[If $U$ is empty $M$ can't transition, at least 1 input is required, i.e. $epsilon$]
- $X = {0, 1, 2, 3, 4}$ 
- $Y = "Cost" times "Duration"$
- $x_0 = 0$

$
  g(x) = cases(
    (0, 0) & quad x = 0 or x = 4 \
    (20000, 2) & quad x = 1 or x = 3 \
    (40000, 4) & quad x = 2
  )
$

#align(center)[
  #figure({
    set text(font: "CaskaydiaCove NF", weight: "light", lang: "en")
    image("public/development-process-markov-chain.svg")
  }, caption: "the model of a team's development process") <development-process> 
]

#v(1em)

#align(center)[
  #math.equation[
    p = 
    #table(
      columns: (auto, auto, auto, auto, auto, auto),
      stroke: .1pt,
      align: center + horizon,
      table.header([$epsilon$], [*0*], [*1*], [*2*], [*3*], [*4*]),
      [*0*], [0], [1], [0], [0], [0],
      [*1*], [0], [.3], [.7], [0], [0],
      [*2*], [0], [.1], [.2], [.7], [0],
      [*3*], [0], [.1], [.1], [.1], [.7],
      [*4*], [1], [0], [0], [0], [0],
    )
  ]
]

#v(1em)

Only *1 transition matrix* is defined, as $|U| = 1$ (there's 1 input value). If $U$ had multiple input values, like ${"apple", "banana", "orange"}$, then 3 transition matrices would have been required, one *for each input value*.

=== Network of MDPs

Let $M_1, M_2$ be two MDPs s.t. $M_1 = (U_1, X_1, Y_1, p_1, g_1)$ and $M_2 = (U_2, X_2, Y_2, p_2, g_2)$, then $M = (U_1, X_1 times X_2, Y_2, p, g)$ s.t. etc... is a MDP. #footnote[It's not so easy to describe, I'll work on it later, TODO]

#pagebreak()

== Other methods // Tips and tricks

=== Incremental average

Given a set of values $X = {x_1, ..., x_n} subset RR$ the average $overline(x)_n = (sum_(i = 0)^n x_i) / n$ can be computed with a simple procedure 

```cpp
float average(std::vector<float> X) {
    float sum = 0;
    for (auto x_i : X)
        sum += x_i;

    return sum / X.size();
}
```

The problem with this procedure is that, by adding up all the values before the division, the *numerator* could *overflow*, even if the value of $overline(x)_n$ fits within the IEEE-754 limits. Nonetheless, $overline(x)_n$ can be calculated incrementally.

$
  overline(x)_(n + 1) = (sum_(i = 0)^(n + 1) x_i) / (n + 1) = 
  ((sum_(i = 0)^n x_i) + x_(n + 1)) / (n + 1) = 
  (sum_(i = 0)^n x_i) / (n + 1) + x_(n + 1) / (n + 1) = \
  ((sum_(i = 0)^n x_i) n) / ((n + 1) n) + x_(n + 1) / (n + 1) = 
  (sum_(i = 0)^n x_i) / n dot.c n / (n + 1) + x_(n + 1) / (n + 1) = \ 
  overline(x)_n dot n / (n + 1) + x_(n + 1) / (n + 1)
$

With this formula the numbers added up are smaller: $overline(x)_n$ is multiplied by $n / (n + 1) tilde 1$, and, if $x_(n + 1)$ fits in IEEE-754, then $x_(n + 1) / (n + 1)$ can also be encoded.

```cpp
float incr_average(std::vector<float> X) {
    float average = 0;
    for (size_t n = 0; n < X.size(); n++)
        average =
            average * ((float)n / (n + 1)) + X[n] / (n + 1);

    return average;
}
```

In `examples/average.cpp` the procedure `average()` returns `Inf` and `incr_average()` successfuly computes the average.

#pagebreak()

=== Welford's online algorithm // (standard deviation)

In a similar fashion, it could be faster and require less memory to calculate the *standard deviation* incrementally. Welford's online algorithm can be used for this purpose. 

$ 
M_(2, n) = sum_(i=1)^n (x_i - overline(x)_n)^2 \
M_(2, n) = M_(2, n-1) + (x_n - overline(x)_(n - 1))(x_n - overline(x)_n) \
sigma^2_n = M_(2, n) / n \
s^2_n = M_(2, n) / (n - 1)
$

Given $M_2$, the standard deviation can be calculated as $sqrt(M_(2, n) / n)$ if $n > 0$.

#figure(caption: `mocc/math.cpp`)[
```cpp
real_t Stat::stddev() const {
    return n > 0 ? sqrt(m_2__ / n) : 0;
}
```
]

=== Euler method // for ordinary differential equations

When an ordinary differential equation can't be solved analitically, the solution must be approximated. There are many techniques: one of the simplest ones (yet less accurate and efficient) is the forward Euler method, described by the following equation:

$ y_(n + 1) = y_n + Delta dot.c f(x_n, y_n) $ <euler-method>

Let the function $y$ be the solution to the following problem 

$ 
cases(
  y(x_0) = y_0 \
  y'(x) = f(x, y(x))
) 
$

Let $y(x_0) = y_0$ be the initial condition of the system, and $y' = f(x, y(x))$ be the known *derivative* of $y$ ($y'$ is a function of $x$ and $y(x)$). To approximate $y$, a $Delta$ is chosen (the smaller, the more precise the approximation), then $x_(n + 1) = x_n + Delta$. Now, understanding @euler-method should be easier: the value of $y$ at the next *step* is the current value of $y$ plus the value of its derivative $y'$ (multiplied by $Delta$). In @euler-method $y'$ is multiplied by $Delta$ because when going to the next step, all the derivatives from $x_n$ to $x_(n + 1)$ must be added up, and it's done by adding up 

$ (x_(n + 1) - x_n) dot.c f(x_n, y_n) = Delta dot.c f(x_n, y_n) $

Where $y_n = y(x_n)$. Given this theoretical understanding, implementing the code should be simple enough.

#pagebreak()

The following program approximates $cases(y(x_0) = 0 \ y'(x) = 2x)$ with $Delta = 1/1, 1/2, 1/3, 1/4$ //, knowing that $y' = 2x$.

#figure(caption: `examples/euler.cpp`)[
```cpp
#define SIZE 4

float derivative(float x) { 
    return 2 * x; 
}

int main() {
    size_t x[SIZE];

    for (size_t i = 0; i < SIZE; i++) {
        x[i] = 0;
        float delta = 1. / (i + 1);
        for (float t = 0; t <= 10; t += delta) 
            x[i] += delta * derivative(t);
    }

    return 0;
}
```
]

When plotting the results, it can be observed that the approximation is close, but not very precise. The error analysis in the Euler method is beyond this guide's scope. 

#align(center)[
  #figure(caption: "examples/euler.svg")[
    #image("examples/euler.svg", width: 99%)
  ]
]

=== Monte Carlo method

The Monte Carlo method is used to approximate values. For example, if you want to approximate the area under a function, just scale the function to a $1 times 1$ square, generate $n$ random x points, count how many of them are under the curve (let that number be $s$) and the area is $s / n$. It can also be used to approximate $pi$ for example.

The Monte Carlo method can be used to approximate probabilities:
- run a simulation $n$ times (where $n$ is very big)
- for each simulation
  - get the cost of the simulation 
  - add $1$ to $s$ if the cost is below a certain threshold $t$ 
- the probability that the cost of the system is below $t$ is $s / n$
- TODO: this can be seen in `practice/etc...`

#pagebreak()

= How to `C++`

This section covers the basics assuming the reader already knows `C`.

== The ```cpp random``` library <random-library>

The `C++` standard library offers tools to easily implement MDPs.

=== Random engines 

In `C++` there are many ways to *generate random numbers* @pseudo-random-number-generation. Generally it's not recommended to use ```cpp random()``` #reft(1) /*(reasons...)*/. It's better to use a *random generator* #reft(5), because it's fast, deterministic (given a seed, the sequence of generated numbers is the same) and can be used with distributions. A `random_device` is a non deterministic generator: it uses a *hardware entropy source* (if available) to generate the random numbers.

#figure(caption: `examples/random.cpp`)[
```cpp
#include <iostream>
#include <random>

int main() {
    std::cout << random() t1 << std::endl;

    std::random_device random_device; t2
    std::cout << random_device() t3 << std::endl;

    int seed = random_device(); t4
    std::default_random_engine random_engine(seed); t5
    std::cout << random_engine() t6 << std::endl;
}
```
] <random-example>

The typical course of action is to instantiate a `random_device` #reft(2), and use it to generate a seed #reft(4) for a `random_engine`.  Given that random engines can be used with distributions, they're really useful to implement MDPs. Also, #reft(3) and #reft(6) are examples of *operator overloading* (@operator-overloading).

From this point on, ```cpp std::default_random_engine``` will be reffered to as ```cpp urng_t``` (uniform random number generator type).

#figure()[
```cpp
#include <random>
// works like typedef in C
using urng_t = std::default_random_engine; 

int main() {
    urng_t urng(190201); 
}
```
]

=== Distributions <distributions>

Just the capability to generate random numbers isn't enough, these numbers need to be manipulated to fit certain needs. Luckly, `C++` covers *basically all of them*. For example, the MDP in @development-process can be easily simulated with the following code code:

#figure(caption: `examples/transition_matrix.cpp`)[
```cpp
#include <iostream>
#include <random>
using urng_t = std::default_random_engine;

int main() {
    std::random_device random_device;
    urng_t urng(random_device());

    std::discrete_distribution<> transition_matrix[] = {
        {0, 1},
        {0, .3, .7},
        {0, .2, .2, .6},
        {0, .1, .2, .1, .6},
        {1},
    };

    size_t state = 0;
    for (size_t step = 0; step < 15; step++) {
        state = transition_matrix[state](urng);
        std::cout << state << std::endl;
    }

    return 0;
}
```
] <transition-matrix>

==== Uniform discrete @docs-uniform-int-distribution <uniform-int>

#note[
To test a system $S$ it's requried to build a generator that sends value $v_t$ to $S$ every $T_t$ seconds. For each send, the value of $T_t$ is an *integer* chosen uniformly in the range $[20, 30]$.
]
The `C` code to compute $T_t$ would be ```cpp T = 20 + rand() % 11;```, which is very *error prone*, hard to remember and has no semantic value. In `C++` the same can be done in a *simpler* and *cleaner* way:

#align(center)[
```cpp
std::uniform_int_distribution<> random_T(20, 30); t1
size_t T = t2 random_T(urng); 
```
]

The interval $T_t$ can be easily generated #reft(2) without needing to remember any formula or trick. The behaviour of $T_t$ is defined only once #reft(1), so it can be easily changed without introducing bugs or inconsistencies. It's also worth to take a look at the implementation of the exercise above (with the addition that $v_t = T_t$), as it comes up very often in software models.

#figure(caption: `examples/interval_generator.cpp`)[
```cpp
#include <iostream>
#include <random>

using urng_t = std::default_random_engine;

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::uniform_int_distribution<> random_T(20, 30);

    size_t T = random_T(urng), next_request_time = T;
    for (size_t time = 0; time < 1000; time++) {
        if (time < next_request_time)
            continue;

        std::cout << T << std::endl;
        T = random_T(urng);
        next_request_time = time + T;
    }

    return 0;
}
```
]

The ```cpp uniform_int_distribution``` has many other uses, for example, it could uniformly generate a random state in a MDP. Let ```cpp STATES_SIZE``` be the number of states

#align(center)[
```cpp 
uniform_int_distribution<> random_state(0, STATES_SIZE - 1 t1);
``` 
]

```cpp random_state``` generates a random state when used. Be careful! Remember to use ```cpp STATES_SIZE - 1``` #reft(1), because ```cpp uniform_int_distribution``` is inclusive. Forgettig ```cpp -1``` can lead to very sneaky bugs, like random segfaults at different instructions. It's very hard to debug unless using `gdb`. The ```cpp uniform_int_distribution``` can also generate negative integers, for example $z in { x | x in ZZ and x in [-10, 15]}$. 

==== Uniform continuous @docs-uniform-real-distribution <uniform-real>

It's the same as above, with the difference that it generates *real* numbers in the range $[a, b) subset RR$. 

==== Bernoulli @docs-bernoulli-distribution <bernoulli>

#note[
  To model a network protocol $P$ it's necessary to model requests. When sent, a request can randomly fail with probability $p = 0.001$.
]

Generally, a random fail can be simulated by generating $r in [0, 1]$ and checking whether $r < p$.

```cpp 
std::uniform_real_distribution<> uniform(0, 1);
if (uniform(urng) < 0.001) 
    fail();
```

A `std::bernoulli_distribution` is a better fit for this specification, as it generates a boolean value and its semantics represents "an event that could happen with a certain probability $p$".

```cpp 
std::bernoulli_distribution random_fail(0.001);
if (random_fail(urng)) 
    fail();
```

==== Normal <normal>

==== Exponential <exponential>

The Exponential distribution is very useful when simulating user requests (generally, the interval between requests to a servers is described by a Exponential distribution, you just have to specify $lambda$)

==== Poisson <poisson>

==== Geometric <geometric>

==== Discrete distribution <discrete>

#note[
  To choose the architecture for an e-commerce it's necessary to simulate realistic purchases. After interviewing 678 people it's determined that 232 of them would buy a shirt from your e-commerce, 158 would buy a hoodie and the other 288 would buy pants. 
]

The objective is to simulate random purchases reflecting the results of the interviews. One way to do it is to calculate the percentage of buyers for each item, generate $r in [0, 1]$, and do some checks on $r$. However, this specification can be implemented very easily in `C++` by using a ```cpp std::discrete_distribution```, without having to do any calculation or write complex logic.

// #align(center)[
#figure(caption: `examples/TODO.cpp`)[
```cpp
enum Item { Shirt = 0, Hoodie = 1, Pants = 2 };

int main() { 
    std::discrete_distribution<> 
        rand_item = {232, 158, 288}; t1

    for (size_t request = 0; request < 1000; request++) {
        switch (rand_item(urng)) {
            case Shirt: t2 std::cout << "shirt"; break;
            case Hoodie: t2 std::cout << "hoodie"; break;
            case Pants: t2 std::cout << "pants"; break;
        }

        std::cout << std::endl;
    }

    return 0;
}
```
]

- TODO:
  - #reft(1) why can the {a, b, c} syntax be used
  - #reft(1) the weight thing and the formula (accumulated sums prob)
  - #reft(2) how can enums be used here
  - with the discrete distribution, the generated items are proportional to the data.

== Data 

=== Manual memory allocation

If you allocate with ```cpp new```, you must deallocate with ```cpp delete```, you can't mixup them with ```c malloc()``` and ```c free()```

To avoid manual memory allocation, most of the time it's enough to use the structures in the standard library, like ```cpp std::vector<T>```.

=== Data structures

==== Vectors <std-vector>
// === ```cpp std::vector<T>()``` <std-vector>

You don't have to allocate memory, basically never! You just use the structures that are implemented in the standard library, and most of the time they are enough for our use cases. They are really easy to use.

==== Deques <std-deque>
// === ```cpp std::deque<T>()``` <std-deque>

==== Sets <std-set>

Not needed as much

==== Maps <std-map>

Could be useful

== I/O

=== Standard I/O  <iostream>

=== Files <files>

Working with files is way easier in `C++`

```cpp
#include <fstream>

int main(){
    std::ofstream output("output.txt");
    std::ifstream params("params.txt");

    while (etc...) {}

    output.close();
    params.close();
}
```
//
// #align(center)[
// ```cpp
// #include <ofstream>
// #include <ifstream>
//
// int main(){ 
//     ofstream output("output.txt");
//     output << "some text" << std::endl;
//     output.close();
//
//     ifstream params("params.txt");
//     int number;
//     while (params >> number) {
//         // do stuff with number...
//     }
//     params.close();
//
//     return 0;
// }
// ```
// ]

== Operator overloading <operator-overloading>

In @random-example, to generate a random number, ```cpp random_device()``` #reft(3) and ```cpp random_engine()``` #reft(6) are used like functions, but they aren't functions, they're instances of a ```cpp class```. That's because in `C++` you can define how a certain operator (like `+`, `+=`, `<<`, `>>`, `[]`, `()` etc..) should behave when used on a instance of the ```cpp class```. 
It's called *operator overloading*, a relatively common feature: 
- in `Python` operation overloading is done by implementing methods with special names, like ```python __add__()``` @python-operator-overloading
- in `Rust` it's done by implementing the `Trait` associated with the operation, like ```rust std::ops::Add``` @rust-operator-overloading.
- `Java` and `C` don't have operator overloading

For example, ```cpp std::cout``` is an instance of the ```cpp std::basic_ostream class```, which overloads the method "```cpp operator<<()```" @basic-ostream. The same applies to to file streams.

== Code structure

=== Classes

- TODO:
  - Maybe constructor
  - Maybe operators? (more like nah)
  - virtual stuff (interfaces)

=== Structs

- basically like classes, but with everything public by default 

=== Enums

- enum vs enum class
- an example maybe 
- they are useful enough to model a finite domain

=== Inheritance

#pagebreak()

= Debugging with `gdb`

It's super useful! Trust me, if you learn this everything is way easier (printf won't be useful anymore)

First of all, use the `-ggdb3` flags to compile the code. Remember to not use any optimization like `-O3`... using optimizations makes the program harder to debug.

```makefile
DEBUG_FLAGS := -lm -ggdb3 -Wall -Wextra -pedantic
```

Then it's as easy as running `gdb ./main`

- TODO: could be useful to write a script if too many args
- TODO: just bash code to compile and run
- TODO (just the most useful stuff, thechnically not enough):
  - r
  - c
  - n
  - c 10
  - enter (last instruction)
  - b
    - on lines
    - on symbols
    - on specific files
  - clear
  - display
  - set print pretty on


#pagebreak()

= Examples 

Each example has 4 digits `xxxx` that are the same as the ones in the `software` folder in the course material. The code will be *as simple as possible* to better explain the core functionality, but it's *strongly suggested* to try to add structure _(classes etc..)_ where it *seems fit*.

== First examples

This section puts together the *formal definitions* and the `C++` knowledge to implement some simple MDPs.

=== A simple MDP `[1100]` <a-simple-markov-chain>

The first MDP $M = (U, X, Y, p, g)$ is such that
- $U = {epsilon}$ #footnote[See @mdp-example]
- $X = [0,1] times [0,1]$, each state is a pair #reft(3) of *real* numbers #reft(2) 
- $Y = [0,1] times [0,1]$
- $p : X times X times U -> X = cal(U)(0, 1) times cal(U)(0, 1)$, the transition probability is a *uniform continuous* distribution #reft(1)
- $g : X -> Y : (r_0, r_1) |-> (r_0, r_1)$ outputs the current state #reft(4)
- $x_0 = (0, 0)$ is the initial state #reft(3)

#figure(caption: `software/1100/main.cpp`)[
```cpp
int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::uniform_real_distribution<real_t> uniform(0, 1); t1

    std::vector<real_t t2 > state(2, 0); t3
    std::ofstream log("log");

    for (size_t time = 0; time <= HORIZON; time++) {
        for (auto &r : state) 
            r = uniform(urng); t1

        log << time << ' ';
        for (auto r : state) 
            log << r << ' '; t t4
        log << std::endl;
    }

    log.close();
    return 0;
}
```
]

=== MDPs network pt.1 `[1200]` <simple-mdps-connection-1>

This example has 2 MDPs $M_0, M_1$ s.t.
- $M_0 = (U^0, X^0, Y^0, p^0, g^0)$ 
- $M_1 =(U^1, X^1, Y^1, p^1, g^1)$ 

$M_0$ and $M_1$ are similar to the MDP in @a-simple-markov-chain, with the difference that $U^i = [0, 1] times [0, 1]$, having $U^i = X^i$, meaning $p$ must be redefined:

$ p^i (x'|x, u) = cases(1 & "if" x' = u \ 0 & "otherwise") $

Then the 2 MDPs can be connected 

$
  & U^0_(t + 1) = (r_0 dot.c cal(U)(0, 1), r_1 dot.c cal(U)(0, 1)) "where" g^1 (X^1_t) = (r_0, r_1) \
  & U^1_(t + 1) = (r_0 + cal(U)(0, 1), r_1 + cal(U)(0, 1)) "where" g^0 (X^0_t) = (r_0, r_1)
$ <mdps-connection-1>

Given that $g^i (X^i_t) = X^i_t$ and $U^i_t = X^i_t$, the connection in @mdps-connection-1 can be simplified:

$
  & X^0_(t + 1) = (r_0 dot.c cal(U)(0, 1), r_1 dot.c cal(U)(0, 1)) "where" X^1_t = (r_0, r_1) \
  & X^1_(t + 1) = (r_0 + cal(U)(0, 1), r_1 + cal(U)(0, 1)) "where" X^0_t = (r_0, r_1)
$ <mdps-connection-2>

With @mdps-connection-2 the code is easier to write, but this approach works for small examples like this one. For more complex systems it's better to design a module for each component and handle the connections more explicitly.

#figure(caption: `software/1200/main.cpp`)[
```cpp
const size_t HORIZON = 100;
struct MDP { real_t state[2]; };

int main() {
    std::vector<MDP> mdps(2, {0, 0});

    for (size_t time = 0; time <= HORIZON; time++) 
        for (size_t r = 0; r < 2; r++) {
            mdps[0].state[r] = mdps[1].state[r] * uniform(urng);
            mdps[1].state[r] = mdps[0].state[r] + uniform(urng);
        }
}
```
]

=== MDPs network pt.2 `[1300]`

This example is similar to the one in @simple-mdps-connection-1, with a few notable differences:
- $U^i = X^i = Y^i = RR times RR$
- the initial states are $x^0_0 = (1, 1), x^1_0 = (2, 2)$
- the connections are slightly more complex.
- no probability is involved

Having 

$ p((x_0 ', x_1 ')|(x_0, x_1), (u_0, u_1)) = cases(1 & "if" ... \ 0 & "otherwise" ) $

The implementation would be

#figure(caption: `software/1300/main.cpp`)[
```cpp
int main() {
    std::vector<MDP> mdps({{1, 1}, {2, 2}});

    for (size_t time = 0; time <= HORIZON; time++) {
        mdps[0].state[0] =
            .7 * mdps[0].state[0] + .7 * mdps[0].state[1];
        mdps[0].state[1] =
            -.7 * mdps[0].state[0] + .7 * mdps[0].state[1];

        mdps[1].state[0] =
            mdps[1].state[0] + mdps[1].state[1];
        mdps[1].state[1] =
            -mdps[1].state[0] + mdps[1].state[1];
    }
}
```
] <mdps-connection-3>

=== MDPs network pt.3 `[1400]`

The original model behaves exactly lik @mdps-connection-3, with a different implementation. As an exercise, the reader is encouraged to come up with a different implementation for @mdps-connection-3.

== Traffic light `[2000]`

This example models a *traffic light*. The three original versions (`2100`, `2200` and `2300`) have the same behaviour, with a different implementation.

Let $T$ be the MDP that describes the traffic light, s.t.
- $U = {epsilon, sigma}$ where 
  - $epsilon$ means "do nothing" 
  - $sigma$ means "switch light"
- $X = {"green", "yellow", "red"}$
- $Y = X$
- $g(x) = x$
- $p(x'|x, epsilon) = cases(1 & "if " x' = x \ 0 & "otherwise")$
- $p(x'|x, sigma) = cases(
  1 & "if " (x = "green" and x' = "yellow") or (x = "yellow" and x' = "red") or (x ="red" and x' = "green") \ 
  0 & "otherwise"
)$

Meaning that, if the input is $epsilon$, $T$ maintains the same color with probability 1. Otherwise, if the input is $sigma$, $T$ changes color with probability 1, iif the transition is valid (green $->$ yellow, yellow $->$ red, red $->$ green)

#figure(caption: `software/2000/main.cpp`)[
```cpp
#include <fstream>
#include <random>

using real_t = double;
using urng_t = std::default_random_engine;
const size_t HORIZON = 1000;

enum Light { GREEN = 0, YELLOW = 1, RED = 2 }; t1

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::uniform_int_distribution<> rand_interval(60, 120); t2
    std::ofstream log("log");

    Light traffic_light = Light::RED;
    size_t next_switch = rand_interval(urng);

    for (size_t time = 0; time <= HORIZON; time++) {
        log << time << ' ' << next_switch - time << ' '
            << traffic_light << std::endl;

        if (time < next_switch)
            continue;

        traffic_light = t3
            (traffic_light == RED
                 ? GREEN
                 : (traffic_light == GREEN ? YELLOW : RED));

        next_switch = time + rand_interval(urng);
    }

    log.close();
    return 0;
}
```
]

TODO: 
  - #reft(1) ```cpp enum``` vs ```cpp enum class```
  - #reft(2) reference the same trick used in the uniform int distribution example
  - #reft(3) is basically the behaviour of the formula described above
  - how is the time rapresented?
  - how can it be implemented with ```cpp mocc```?


== Control center

This example adds complexity to the traffic light by introducing a *remote control center*, network faults and repairs. It requries some time (it has too many variants), I'll work on it later.

=== No network `[3100]`

=== Network monitor 

==== No faults `[3200]`

==== Faults & no repair `[3300]`

=== Faults & repair `[3400]`

=== Faults & repair + correct protocol `[3500]`

== Statistics

=== Expected value `[4100]`

In this one we just simulate a development process (phase 0, phase 1, and phase 2), and we calculate the average ...


=== Probability `[4200]`

In this one we simulate a more complex software developmen process, and we calculate the average cost (Wait, what? Do we simulate it multiple times?)

#pagebreak()

== Development process simulation 

An MDP can be implemented by using a *transition matrix* (like in @mdp-example). The simplest implemenation can be done by using a ```cpp std::discrete_distribution``` by using the trick in @transition-matrix.

=== Random transition matrix `[5100]`

This example builds a *random transition matrix*. 

#figure(caption: `software/5100/main.cpp`)[
```cpp
const size_t HORIZON = 20, STATES_SIZE = 10;

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    auto random_state = t1
        std::uniform_int_distribution<>(0, STATES_SIZE - 1);
    std::uniform_real_distribution<> random_real_0_1(0, 1);

    std::vector<std::discrete_distribution<>>
        transition_matrix(STATES_SIZE); t2
    std::ofstream log("log.csv");

    for (size_t state = 0; state < STATES_SIZE; state++) {
        std::vector<real_t> weights(STATES_SIZE); t3
        for (auto &weight : weights)
            weight = random_real_0_1(urng);

        transition_matrix[state] = t4
            std::discrete_distribution<>(weights.begin(),
                                         weights.end());
    }

    size_t state = random_state(urng);
    for (size_t time = 0; time <= HORIZON; time++) {
        log << time << " " << state << std::endl;
        state = transition_matrix[state t5 ](urng); t6
    }

    log.close();
    return 0;
}
```
]

A *transition matrix* is a ```cpp vector<discrete_distribution<>>``` #reft(2) just like in @transition-matrix. Why can we do this? First of all, the states are numbered from ```cpp 0``` to ```cpp STATES_SIZE - 1```, that's why we can generate a random state #reft(1) just by generating a number from ```cpp 0``` to ```cpp STATES_SIZE - 1```. 

The problem with using a simple ```cpp uniform_int_distribution``` is that we don't want to choose the next state uniformly, we want to do something like in @simple-markov-chain.

#figure(
  {
  set text(font: "CaskaydiaCove NF", weight: "light", lang: "en")
  image("public/markov-chain.svg")
  },
  caption: "A simple Markov Chain"
) <simple-markov-chain>

Luckly for us ```cpp std::discrete_distribution<>``` does exactly what we want. It takes a list of weights $w_0, w_1, w_2, ..., w_n$ and assigns each index $i$ the probability $p(i) = (sum_(i = 0)^n w_i) / w_i$ (the probability is proportional to the weight, so we have that $sum_(i = 0)^n p(i) = 1$ like we would expect in a Markov Chain).

To instantiate the ```cpp discrete_distribution``` #reft(4), unlike in @transition-matrix, we need to first calculate the weights #reft(3), as we don't know them in advance.

To randomly generate the next state #reft(6) we just have to use the ```cpp discrete_distribution``` assigned to the current state #reft(5). 

=== `[5200]` Software development & error detection

Our next goal is to model the software development process of a team. Each phase takes the team 4 days to complete, and, at the end of each phase the testing team tests the software, and there can be 3 outcomes:
- *no error* is introduced during the phase (we can't actually know it, let's suppose there is an all-knowing "oracle" that can tell us there aren't any errors)
- *no error detected* means that the "oracle" detected an error, but the testing team wasn't able to find it
- *error detected* means that the "oracle" detected an error, and the testing team was able to find it

If we have *no error*, we proceed to the next phase... the same happens if *no error was detected* (because the testing team sucks and didn't find any errors). If we *detect an error* we either reiterate the current phase (with a certain probability, let's suppose $0.8$), or we go back to one of the previous phases with equal probability (we do this because, if we find an error, there's a high chance it was introduced in the current phase, and we want to keep the model simple).

In this exercise we take the parameters for each phase (the probability to introduce an error and the probability to not detect an error) from a file.

#figure(caption: `software/5300/main.cpp`)[ 
```cpp
#include <...>

using real_t = double;
const size_t HORIZON = 800, PHASES_SIZE = 3;

enum Outcome t1 {
    NO_ERROR = 0,
    NO_ERROR_DETECTED = 1,
    ERROR_DETECTED = 2
};

int main() {
    std::random_device random_device;
    std::default_random_engine urng(random_device());
    std::uniform_real_distribution<> uniform_0_1(0, 1);
    std::vector<std::discrete_distribution<>>
        phases_error_distribution;

    {
        std::ifstream probabilities("probabilities.csv");
        real_t probability_error_introduced,
            probability_error_not_detected;

        while (probabilities >> probability_error_introduced >>
               probability_error_not_detected)
            phases_error_distribution.push_back(
                t2 std::discrete_distribution<>({
                    1 - probability_error_introduced,
                    probability_error_introduced *
                        probability_error_not_detected,
                    probability_error_introduced *
                        (1 - probability_error_not_detected),
                }));

        probabilities.close();
        assert(phases_error_distribution.size() ==
               PHASES_SIZE);
    }

    real_t probability_repeat_phase = 0.8;

    size_t phase = 0;
    std::vector<size_t> progress(PHASES_SIZE, 0);
    std::vector<Outcome> outcomes(PHASES_SIZE, NO_ERROR);

    for (size_t time = 0; time < HORIZON; time++) {
        progress[phase]++;

        if (progress[phase] == 4) {
            outcomes[phase] = static_cast<Outcome>(
                phases_error_distribution[phase](urng));
            switch (outcomes[phase]) {
            case NO_ERROR:
            case NO_ERROR_DETECTED:
                phase++;
                break;
            case ERROR_DETECTED:
                if (phase > 0 && uniform_0_1(urng) >
                                     probability_repeat_phase)
                    phase = std::uniform_int_distribution<>(
                        0, phase - 1)(urng);
                break;
            }

            if (phase == PHASES_SIZE)
                break;

            progress[phase] = 0;
        }
    }

    return 0;
}
```
] <error-detection>

TODO: ```cpp class enum``` vs ```cpp enum```. We can model the outcomes as an ```cpp enum``` #reft(1)... we can use the ```cpp discrete_distribution``` trick to choose randomly one of the outcomes #reft(2). The other thing we notice is that we take the probabilities to generate an error and to detect it from a file.

#pagebreak()

=== Optimizing costs for the development team `[5300]`

If we want we can manipulate the "parameters" in real life: a better experienced team has a lower probability to introduce an error, but a higher cost. What we can do is:
1. randomly generate the parameters (probability to introduce an error and to not detect it)
2. simulate the development process with the random parameters
By repeating this a bunch of times, we can find out which parameters have the best results, a.k.a generate the lowest development times (there are better techniques like simulated annealing, but this one is simple enough for us).

=== Key performance index `[5400]`

We can repeat the process in exercise `[5300]`, but this time we can assign a parameter a certain cost, and see which parameters optimize cost and time (or something like that? Idk, I should look up the code again).


== Complex systems

=== Insulin pump `[6100]`

=== Buffer `[6200]`

=== Server `[6300]`

#pagebreak()

= MOCC library

Model CheCking

== Observer Pattern

Basically: the "Observer Pattern" @observer-pattern can be used because a MDP is like an entity that "is notified" when something happens (receives an input, in fact, in the case of MDPs, another name for input is "action"), and notifies other entities (output, or reward)

== `C++` generics & virtual methods

Generics allow to connect MDPs more safely, as the inputs and outputs are typed! (It's still not fault-proof)

#pagebreak()

= Practice

In short, every system can be divided into 4 steps:
- reading parameters from a file (from files as of 2024/2025)
- initializing the system
  - this include instantiating the MDPs and connecting them
- simulating the system
- saving outputs to a file

#figure(caption: `practice/1/main.cpp`)[
```cpp
std::ifstream params("parameters.txt");
char c;

while (params >> c) t1
    switch (c) {
        case 'A': params >> A; break;
        case 'B': params >> B; break;
        case 'C': params >> C; break;
        case 'D': params >> D; break;
        case 'F': params >> F; break;
        case 'G': params >> G; break;
        case 'N': params >> N; break;
        case 'W': params >> W; break;
    }

params.close();
```
]

Reading the input: `std::ifstream` can read (from a file) based on the type of the variable read. For exapmle, `c` is a ```cpp char```, so #reft(1) will read exactly 1 character. If `c` was a string, ```cpp params >> c``` would have read a whole word (up to the first whitespace). For example, `A` is a float and `N` is a int, so ```cpp params >> A``` will try to read a float and ```cpp params >> N``` will *try* to read an int. (TODO: float $->$ real_t, int $->$ size_t)

#figure(caption: `practice/1/parameters.hpp`)[
```cpp
#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include "../../mocc/alias.hpp" t1
#include "../../mocc/mocc.hpp" t2

ALIAS_TYPE(ProjInit, real_t) t3
ALIAS_TYPE(TaskDone, real_t) t3
ALIAS_TYPE(EmplCost, real_t) t3

static t4 real_t A, B, C, D, F, G;
static size_t N, W, HORIZON = 100000;

#endif
```
]

The parameters are declared in a `parameters.hpp` file, for a few reasons
- they are declared globally, and are globally accessible without having to pass to classes constructors
- any class can just import the file with the parameters to access the parameters
- they are static #reft(4) (otherwise clang doesn't like global variables)
- in `parameters.hpp` there are also auxiliary types #reft(3), used in the connections between entities

#align(center)[
```cpp
System system; t1
Stopwatch stopwatch; t2

system.addObserver(&stopwatch); t3

while (stopwatch.elapsed() <= HORIZON) t4
    system.next(); t5
```
]

Simulating the system is actually easy:
- declare the system #reft(1)
- add a stopwatch #reft(2) (which starts from time 0, and everytime the system is updated, it adds up time)
  - it is needed to stop the simulation after a certain amount of time, called `HORIZON`
- connect the stopwatch to the system #reft(3)
- run a loop (like how a game loop would work) #reft(4)
- in the loop, transition the system to the next state #reft(5)

== Development team (time & cost)

=== Employee

#figure(caption: `practice/1/employee.hpp`)[
```cpp
#ifndef EMPLOYEE_HPP_
#define EMPLOYEE_HPP_

#include <random>

#include "../../mocc/stat.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"

class Employee : public Observer<T>,
                 public Observer<ProjInit>,
                 public Notifier<TaskDone, EmplCost> {

    std::vector<std::discrete_distribution<>>
        transition_matrix;
    urng_t &urng;
    size_t phase = 0;
    real_t proj_init = 0;

  public:
    const size_t id;
    const real_t cost;
    Stat comp_time_stat;

    Employee(urng_t &urng, size_t k)
        : urng(urng), id(k),
          cost(1000.0 - 500.0 * (real_t)(k - 1) / (W - 1)) {

        transition_matrix =
            std::vector<std::discrete_distribution<>>(N);

        for (size_t i = 1; i < N; i++) {
            size_t i_0 = i - 1;
            real_t tau = A + B * k * k + C * i * i + D * k * i,
                   alpha = 1 / (F * (G * W - k));

            std::vector<real_t> p(N, 0.0);
            p[i_0] = 1 - 1 / tau;
            p[i_0 + 1] =
                (i_0 == 0 ? (1 - p[i_0])
                          : (1 - alpha) * (1 - p[i_0]));

            for (size_t prev = 0; prev < i_0; prev++)
                p[prev] = alpha * (1 - p[i_0]) / i_0;

            transition_matrix[i_0] =
                std::discrete_distribution<>(p.begin(),
                                             p.end());
        }

        transition_matrix[N - 1] =
            std::discrete_distribution<>{1};
    }

    void update(T t) override {
        if (phase < N - 1) {
            phase = transition_matrix[phase](urng);
            if (phase == N - 1) {
                comp_time_stat.save(t - proj_init);
                notify((real_t)t, cost);
            }
        }
    };

    void update(ProjInit proj_init) override {
        this->proj_init = proj_init;
        phase = 0;
    };
};

#endif
```
]

=== Director

== Task management

=== Worker

=== Generator

=== Dispatcher (not the correct name)

=== Manager (not the correct name)


== Backend load balancing

=== Env

=== Dispatcher, Server and Database

=== Response time

== Heater simulation

#pagebreak()

= Extras

== VDM (Vienna Development Method)

_"The Vienna Development Method (VDM) is one of the longest established model-oriented formal methods for the development of computer-based systems and software. It consists of a group of mathematically well-founded languages and tools for expressing and analyzing system models during early design stages, before expensive implementation commitments are made. The construction and analysis of the model help to identify areas of incompleteness or ambiguity in informal system specifications, and provide some level of confidence that a valid implementation will have key properties, especially those of safety or security. VDM has a strong record of industrial application, in many cases by practitioners who are not specialists in the underlying formalism or logic. Experience with the method suggests that the effort expended on formal modeling and analysis can be recovered in reduced rework costs arising from design errors."_ @vdm-overture

=== It's cool, I promise 

- Alloy? Maybe it's a good alternative, haven't tried it enough 
- VDM is basically OCL (Object Constraint Language) but better.

=== VDM++ to design valid UMLs


== Advanced testing techinques (in `Rust` & `C`)

- TODO: cite "Rust for Rustaceans"
- TODO: unit tests aren't the only type of test

=== Mocking (mockall)

=== Fuzzying (cargo-fuzz)

=== Property-based testing 

=== Test augmentation (Miri, Loom, Valgrind)

=== Performance testing

- Rust is very focused on performance
- TODO: non-functional requirements

== UI testing?

=== Playwright

== Model checking with Bevy (`Rust`)

#page(bibliography("bibliography.bib"))

// To be more precise, let $x_t$ be the state at the timestep $t$, then 
//
// $ p(x'|x, u) = p(x_(t_(n + 1)) = x' | x_(t_n) = x, u_(t_n) = u) $ 
//
// if the MDP is discrete-time.
// #show heading: set block(above: 1.4em, below: 1em, sticky: false)
// In this course *Markov decision processes* are used to describe software systems.
// *non-functional requirement*


// TODO: don't use "using namespace std;"

// #show raw.where(block: true): block.with(inset: .5em)
// #let note(body) = block(inset: 1em,  body)
// #let note(body) = block(inset: 1em, stroke: silver, body)
// #show raw.where(block: true): set text("New Computer Modern")

// #show figure: set block(breakable: true)
// #show figure.where(kind: raw): set block(width: 100%, fill: luma(254), stroke: (left: 5pt + luma(240)))
// #show raw.where(block: true): block.with(inset: 1em)
// #show raw.where(block: true): block.with(inset: 1em)

// #show figure.where(kind: raw): 
// #show raw.where(block: true): block.with(inset: 1em, width: 100%, fill: luma(254), stroke: (left: 5pt + luma(245)))
// #show align.where(alignment: center): it => {
//   show raw.where(block: true): block.with(inset: 1em, width: 100%, storke: none, fill: white)
//   set block(width: 10pt)
// }

    // box(width: 6pt, place(dx: -1pt, dy: -7pt, 
    //   box(radius: 100%, width: 8pt, height: 8pt, inset: 1pt, fill: black, // stroke: .2pt, 
    //     align(center + horizon, text(white, size: 6pt, strong()))
    //   )
    // ))

    // box(circle(width: .65em, fill: black,
    //   align(center + horizon, text(white, size: .6em, strong(repr(it).slice(2, -1))))
    // )) 

    // text(size: .9em, strong("[" + repr(it).slice(2, -1) + "]"))
    // box(width: .7em, height: .7em, clip: true, radius: 100%,  fill: black,
    //   align(center + horizon, text(white, size: .6em, strong(repr(it).slice(2, -1))))
    // )
    // box(align(center + horizon, 
    // ))
    // box(baseline: .0em, circle(stroke: none, inset: 0em, align(center + horizon, 
    //   text(size: .8em, strong(repr(it).slice(2, -1)))
    // )))

// #let reft(reft) = text(size: .8em, font: "CaskaydiaCove NF", strong("[" + str(reft) + "]"))
// #let reft(reft) = box(circle(width: .65em, fill: black,
//       align(center + horizon, text(white,font: "CaskaydiaCove NF", size: .6em, strong([#reft])))
//     )) 

// box(circle(width: .65em, fill: black,
//       align(center + horizon, text(white,font: "CaskaydiaCove NF", size: .6em, strong([#reft])))
//     )) 


// box(width: 9pt, height: 9pt, clip: true, radius: 100%, stroke: .5pt, baseline: 1pt,
//   align(center + horizon, )
// )

// #let reft(reft) = box(width: 9pt, height: 9pt, clip: true, radius: 100%, stroke: .5pt, baseline: 1pt,
//   align(center + horizon, text(font: "CaskaydiaCove NF", size: 6pt, strong(str(reft))))
// )


// _(a live streaming service for Amazon)_how would you go about it?
// The components can be modeled as *Markov decision processes* to derive conclusions on the system.
// To derive conclusions the system can be *simulated* by modeling its components as *Markov decision processes*.

// _"It is often useful to be able to compute the variance in a single pass, inspecting each value $x_i$ only once; for example, when the data is being collected without enough storage to keep all the values, or when costs of memory access dominate those of computation."_ (#link("https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Online_algorithm")[Wikpedia])

// == Dynamic structures 
// === Manual memory allocation _(and how to *avoid* it)_
// === ```cpp new```, ```cpp delete``` vs ```cpp malloc()``` and ```cpp free()```
// #show raw.where(block: true): block.with(inset: 1em)
