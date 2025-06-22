#set text(font: "New Computer Modern", lang: "en", weight: "light", size: 11pt)
#set page(margin: 1.75in)
#set par(leading: 0.55em, spacing: 0.85em, first-line-indent: 1.8em, justify: true)
#set heading(numbering: "1.1")
#set math.equation(numbering: "(1)")

#show figure: set block(breakable: true)
#show figure.caption: set align(center)
#show heading: set block(above: 1.4em, below: 1em)
#show outline.entry.where(level: 1): it => {
    show repeat: none
    v(1.1em, weak: true)
    text(size: 1em, strong(it))
}
#show raw.where(block: true): block.with(
    inset: 1em,
    width: 100%,
    fill: luma(254),
    stroke: (left: 5pt + luma(245), rest: 1pt + luma(245)),
)
#show figure.where(kind: raw): it => {
    set align(left)
    it
}
#show raw: set text(font: "CaskaydiaCove NFM", lang: "en", weight: "light", size: 9pt)
#show sym.emptyset: sym.diameter

#let reft(reft) = box(
    width: 9pt,
    place(
        dy: -8pt,
        dx: -0pt,
        box(
            radius: 100%,
            width: 9pt,
            height: 9pt,
            inset: 1pt,
            stroke: .5pt, // fill: black,
            align(center + horizon, text(font: "CaskaydiaCove NFM", size: 7pt, repr(reft))),
        ),
    ),
)

#show raw: r => {
    show regex("t(\d)"): it => { reft(int(repr(it).slice(2, -1))) }
    r
}

// #let note(body) = block(width: 100%, inset: 1em, stroke: (paint: silver, dash: "dashed"), body)
#let note(body) = box(width: 100%, inset: 1em, fill: luma(254), stroke: (paint: silver, dash: "dashed"), body)
// #let note(body) = block(inset: 1em, fill: luma(254), stroke: (thickness: 1pt, paint: luma(245)), body)

#let docs(dest) = link(
    "https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution",
    [`(`#underline(offset: 2pt, `docs`)`)`],
)

#page(
    align(
        center + horizon,
        {
            heading(outlined: false, numbering: none, text(size: 1.5em)[Software Engineering])
            text(size: 1.3em)[
                Cicio Ionuț \
            ]
            align(bottom, datetime.today().display("[day]/[month]/[year]"))
        },
    ),
)

#page[The latest version of the `.pdf` and the referenced material can be found at the following link: #underline(link("https://github.com/CuriousCI/software-engineering")[https://github.com/CuriousCI/software-engineering])]

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

#figure(caption: "audio/video monitoring system process")[#{
        set text(font: "CaskaydiaCove NFM", weight: "light", lang: "en")
        block(
            inset: 1em,
            stroke: 1pt + luma(245),
            fill: luma(254),
            image("public/audio-video-monitor.svg", width: 100%),
        )
    }]

To answer questions about the system, it can be simulated by modeling its components as *Markov decision processes*.

== Models <traffic>

=== Markov chain <markov-chain>

In simple terms, a Markov chain $M$ is described by a set of *states* $S$ and the *transition probability* $p : S times S -> [0, 1]$ such that $p(s'|s)$ is the probability to transition from $s$ to $s'$. The transition probability $p$ is constrained by @markov-chain-constrain

$ forall s in S space.en sum_(s' in S) p(s'|s) = 1 $ <markov-chain-constrain>

A Markov chain (or Markov process) is characterized by memorylesness (called the Markov property), meaning that predictions can be made solely on its present state, and aren't influenced by its history.

#figure(caption: [Example Markov chain with $S = {"rainy", "sunny"}$])[
    #{
        set text(font: "CaskaydiaCove NF", weight: "light", lang: "en")
        block(
            width: 100%,
            inset: 1em,
            fill: luma(254),
            stroke: 1pt + luma(245),
            image("public/weather-system.svg", width: 70%),
        )
    }
] <rainy-sunny>

#figure(caption: [Transition probability of @rainy-sunny])[
    #table(
        columns: (auto, auto, auto),
        stroke: .1pt,
        table.header([], [sunny], [rainy]),
        [sunny], [0.8], [0.2],
        [rainy], [0.5], [0.5],
    )
] <rainy-sunny-transition-matrix>

If a Markov chain $M$ transitions at *discrete time* steps (i.e. the time steps $t_0, t_1, t_2, ...$ are a countable) and the *state space* is countable, then it's called a DTMC (discrete-time Markov chain). There are other classifications for continuous state space and continuous-time.

The Markov process is characterized by a *transition matrix* which describes the probability of certain transitions, like the one in @rainy-sunny-transition-matrix. Later in the guide it will be shown that implementing transition matrices in `C++` is really simple with the `<random>` library.

=== Markov decision process <mdp>

A Markov decision process (MDP), despite sharing the name, is *different* from a Markov chain, because it interacts with an *external environment*. A MDP $M$ is a tuple $(U, X, Y, p, g)$ s.t.
- $U$ is the set of *input values*
- $X$ is the set of *states*
- $Y$ is the set of *output values*
- $p : X times X times U -> [0, 1]$ is such that $p(x'|x, u)$ is the probability to *transition* from state $x$ to state $x'$ when the *input value* is $u$
- $g : X -> Y$ is the *output function*
- $x_0 in X$ is the *initial state*

The same constrain in @markov-chain-constrain holds for MDPs, with an important difference: *for each input value*, the sum of the transition probabilities for *that input value* must be 1.

$ forall x in X space.en forall u in U space.en sum_(x' in X) p(x'|x, u) = 1 $

==== MDP example <mdp-example>

The development process of a company can be modeled as a MDP \ $M = (U, X, Y, p, g)$ s.t.
- $U = {epsilon}$ #footnote[If $U$ is empty $M$ can't transition, at least 1 input is required, i.e. $epsilon$], $X = {0, 1, 2, 3, 4}, Y = "Cost" times "Duration", x_0 = 0$

$
    g(x) = cases(
        (0, 0) & quad x in {0, 4} \
        (20000, 2) & quad x in {1, 3} \
        (40000, 4) & quad x = 2
    )
$

#v(1em)

#align(center)[
    #figure(
        {
            set text(font: "CaskaydiaCove NF", weight: "light", lang: "en")
            block(
                inset: 1em,
                fill: luma(254),
                stroke: 1pt + luma(245),
                image("public/development-process-markov-chain.svg"),
            )
        },
        caption: "the model of a team's development process",
    ) <development-process>
]

#v(1em)

#grid(
    columns: (auto, auto),
    gutter: 1em,
    [
        #table(
            columns: (auto, auto, auto, auto, auto, auto),
            stroke: .1pt,
            align: center + horizon,
            table.header([$epsilon$], [*0*], [*1*], [*2*], [*3*], [*4*]),
            [*0*], [0], [1], [0], [0], [0],
            [*1*], [0], [.3], [.7], [0], [0],
            [*2*], [0], [.1], [.2], [.7], [0],
            [*3*], [0], [.1], [.1], [.1], [.7],
            [*4*], [0], [0], [0], [0], [1],
        )
    ],
    [
        Only *1 transition matrix* is needed, as $|U| = 1$ (there's 1 input value). If $U$ had multiple input values, like ${"on", "off", "wait"}$, then multiple transition matrices would have been required, one *for each input value*.
    ],
)

=== Network of MDPs

Let $M_1, M_2$ be two MDPs s.t.
- $M_1 = (U_1, X_1, Y_1, p_1, g_1)$
- $M_2 = (U_2, X_2, Y_2, p_2, g_2)$

Then $M = (U_1, X_1 times X_2, Y_2, p, g)$ s.t.
- $p((x_1', x_2') | (x_1, x_2), u_1) = (p(x_1'|x_1, u_1), p(x_2'|x_2, g_1(x_1)))$
- $g((x_1, x_2)) = g_2(x_2)$

- TODO the connection can also be partial

#pagebreak()

== Other methods

=== Incremental average <incremental-average>

Given a set of values $X = {x_1, ..., x_n} subset RR$ the average $overline(x)_n = (sum_(i = 0)^n x_i) / n$ can be computed with a simple procedure

#figure(caption: `examples/average.cpp`)[
    ```cpp
    float average(std::vector<float> X) {
        float sum = 0;
        for (auto x_i : X)
            sum += x_i;

        return sum / X.size();
    }
    ```
]

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

#figure(caption: `examples/average.cpp`)[
    ```cpp
    float incr_average(std::vector<float> X) {
        float average = 0;
        for (size_t n = 0; n < X.size(); n++)
            average =
                average * ((float)n / (n + 1)) + X[n] / (n + 1);

        return average;
    }
    ```
]

In `examples/average.cpp` the procedure `incr_average()` successfuly computes the average, but the simple `average()` returns `Inf`.

#pagebreak()

=== Welford's online algorithm <welford>

In a similar fashion, it could be faster and require less memory to calculate the *standard deviation* incrementally. Welford's online algorithm can be used for this purpose.

$
    M_(2, n) = sum_(i=1)^n (x_i - overline(x)_n)^2 \
    M_(2, n) = M_(2, n-1) + (x_n - overline(x)_(n - 1))(x_n - overline(x)_n) #reft(2) \
    sigma^2_n = M_(2, n) / n \
    // s^2_n = M_(2, n) / (n - 1)
$

Given $M_2$, if $n > 0$, the standard deviation is $sqrt(M_(2, n) / n)$ #reft(3) . The average can be calculated incrementally like in @incremental-average #reft(1) .

#figure(caption: `mocc/math.cpp`)[
    ```cpp
    void Stat::save(real_t x_i) {
        real_t next_mean =
            mean_ * ((real_t)n / (n + 1)) + x_i / (n + 1); t1

        m_2__ += (x_i - mean_) * (x_i - next_mean); t2
        mean_ = next_mean;
        n++;
    }

    real_t Stat::stddev() const {
        return n > 0 ? sqrt(m_2__ / n) : 0; t3
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

Let $y(x_0) = y_0$ be the initial condition of the system, and $y' = f(x, y(x))$ be the known *derivative* of $y$ ($y'$ is a function of $x$ and $y(x)$). To approximate $y$, a $Delta$ is chosen (the smaller, the more precise the approximation) s.t. $x_(n + 1) = x_n + Delta$. Now, understanding @euler-method should be easier: the value of $y$ at the *next step* is the current value of $y$ plus the value of its derivative $y'$ (multiplied by $Delta$). In @euler-method $y'$ is multiplied by $Delta$ because when going to the next step, all the derivatives from $x_n$ to $x_(n + 1)$ must be added up, and it's done by adding up

$ (x_(n + 1) - x_n) dot.c f(x_n, y_n) = Delta dot.c f(x_n, y_n) $

Where $y_n = y(x_n)$. Let's consider the example in @euler-method-example.

$
    cases(y(x_0) = 0 \ y'(x) = 2x), quad "with" Delta = 1, 0.5, 0.3, 0.25
$ <euler-method-example>

The following program approximates @euler-method-example with different $Delta$ values.

#figure(caption: `examples/euler.cpp`)[
    ```cpp
    #define SIZE 4
    float derivative(float x) { return 2 * x; }

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

The approximation is close, but not very precise. However, the error analysis is beyond this guide's scope.

#figure(caption: "examples/euler.svg", image("examples/euler.svg", width: 77%))

=== Monte Carlo method

"Monte Carlo methods, or Monte Carlo experiments, are a broad class of computational algorithms that rely on repeated random sampling to obtain numerical results.

The underlying concept is to use randomness to solve problems that might be deterministic in principle [...] Monte Carlo methods are mainly used in three distinct problem classes: optimization, numerical integration, and generating draws from a probability distribution" @monte-carlo-method

#note[
    The cost to develop a feature for a certain software is described by an uniform discrete distribution $cal(U){300, 1000}$. Determine the probability that the cost is less than $550$.
]

The problem above can be easily solved analitically, but let's use the Monte Carlo method to approximate its value.

#figure(caption: `examples/montecarlo.cpp`)[
    ```cpp
    #include <iostream>
    #include <random>

    int main() {
        std::random_device random_device;
        std::default_random_engine rand_engine(random_device());
        std::uniform_int_distribution<> rand_cost(300, 1000);

        const size_t ITERATIONS = 10000;
        size_t below_550 = 0;

        for (size_t i = 0; i < ITERATIONS; i++) t1
            if (rand_cost(rand_engine) < 550) t2
                below_550++; t3

        std::cout << (double)below_550/ITERATIONS t4
                  << std::endl;
        return 0;
    }
    ```
]

The first step is to simulate for a certain number of iterations #reft(1) the system (in this example, "simulating the system" means generating a random integer cost between 300 and 1000 #reft(2) ). If the the iteration respects the requested condition ($< 550$), then it's counted #reft(3).

At the end of the simulations, the probability is calculated as #math.frac([iterations below 550], [total iterations]) #reft(4) . The bigger is the number of iterations, the more precise is the approximation. This type of calculation can be very easily distributed in a HPC (high performance computing) context.

// === Simulated annealing would be cool

// The Monte Carlo method is used to approximate values. For example, if you want to approximate the area under a function, just scale the function to a $1 times 1$ square, generate $n$ random x points, count how many of them are under the curve (let that number be $s$) and the area is $s / n$. It can also be used to approximate $pi$ for example.
//
// The Monte Carlo method can be used to approximate probabilities:
// - run a simulation $n$ times (where $n$ is very big)
// - for each simulation
//   - get the cost of the simulation
//   - add $1$ to $s$ if the cost is below a certain threshold $t$
// - the probability that the cost of the system is below $t$ is $s / n$
// - TODO: this can be seen in `practice/etc...`

#pagebreak()

= How to `C++`

This section covers the basics assuming the reader already knows `C`.

== Prelude

`C++` is a strange language, and some of its quircks need to be pointed out to have a better understanding of what the code does in later sections.

=== Operator overloading <operator-overloading>

#figure(caption: `examples/random.cpp`)[
    ```cpp
    #include <iostream>
    #include <random>

    int main() {
        std::cout << random() t1 << std::endl;

        std::random_device random_device;
        std::cout << random_device() t2 << std::endl;

        int seed = random_device();
        std::default_random_engine random_engine(seed); t3
        std::cout << random_engine() t4 << std::endl;
    }
    ```
] <random-example-1>

In @random-example-1, to generate a random number, ```cpp random_device()``` #reft(2) and ```cpp random_engine()``` #reft(4) are used like functions, but they *aren't functions*, they're *instances* of a ```cpp class```. That's because in `C++` the behaviour a certain operator (like `+`, `+=`, `<<`, `>>`, `[]`, `()` etc..) when used on a instance of the ```cpp class``` can be defined by the programmer.
It's called *operator overloading*, and it's relatively common feature:
- in `Python` operation overloading is done by implementing methods with special names, like ```python __add__()``` @python-operator-overloading
- in `Rust` it's done by implementing the `Trait` associated with the operation, like ```rust std::ops::Add``` @rust-operator-overloading.
- `Java` and `C` don't have operator overloading

For example, ```cpp std::cout``` is an instance of the ```cpp std::basic_ostream class```, which overloads the method "```cpp operator<<()```" @basic-ostream; ```cpp std::cout << "Hello"``` is a valid piece of code which *doesn't do a bitwise left shift* like it would in `C`, but prints on the standard output the string ```cpp "Hello"```. // The same applies to to file streams.

```cpp random()``` #reft(1) _should_ be a regular function call, but, for example, ```cpp std::default_random_engine random_engine(seed);``` #reft(3) is something else alltogether: a constructor call, where ```cpp seed``` is the parameter passed to the constructor to instantiate the ```cpp random_engine``` object.

// === Instantiating objects

#pagebreak()

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

The typical course of action is to instantiate a `random_device` #reft(2), and use it to generate a seed #reft(4) for a `random_engine`. Given that random engines can be used with distributions, they're really useful to implement MDPs. Also, #reft(3) and #reft(6) are examples of *operator overloading* (@operator-overloading).

From this point on, ```cpp std::default_random_engine``` will be reffered to as ```cpp urng_t``` (uniform random number generator type).

#figure()[
    ```cpp
    #include <random>
    // works like typedef in C
    using urng_t = std::default_random_engine;

    int main() {
        urng_t urng(190201); // constructor call with parameter 190201
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
            {0, .1, .2, .7},
            {0, .1, .1, .1, .7},
            {0, 0, 0, 0, 1},
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

==== Uniform discrete distribution #docs("https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution") <uniform-int>

#note[
    To test a system $S$ it's requried to build a generator that sends value $v_t$ to $S$ every $T_t$ seconds. For each send, the value of $T_t$ is an *integer* chosen uniformly in the range $[20, 30]$.
]
The `C` code to compute $T_t$ would be ```cpp T = 20 + rand() % 11;```, which is very *error prone*, hard to remember and has no semantic value. In `C++` the same can be done in a *simpler* and *cleaner* way:

```cpp
std::uniform_int_distribution<> random_T(20, 30); t1
size_t T = t2 random_T(urng);
```

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

```cpp
uniform_int_distribution<> random_state(0, STATES_SIZE-1 t1);
```

```cpp random_state``` generates a random state when used. Be careful! Remember to use ```cpp STATES_SIZE-1``` #reft(1), because ```cpp uniform_int_distribution``` is inclusive. Forgettig ```cpp -1``` can lead to very sneaky bugs, like random segfaults at different instructions. It's very hard to debug unless using `gdb`. The ```cpp uniform_int_distribution``` can also generate negative integers, for example $z in { x | x in ZZ and x in [-10, 15]}$.

==== Uniform continuous distribution #docs("https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution") <uniform-real>

It's the same as above, with the difference that it generates *real* numbers in the range $[a, b) subset RR$.

==== Bernoulli distribution #docs("https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution") <bernoulli>

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

==== Normal distribution #docs("https://en.cppreference.com/w/cpp/numeric/random/normal_distribution") <normal>

Typical Normal distribution, requires the mean #reft(1) and the stddev #reft(2) .

#figure(caption: `examples/normal.cpp`)[
    ```cpp
    #include <iomanip>
    #include <iostream>
    #include <map>
    #include <random>

    using urng_t = std::default_random_engine;

    int main() {
        std::random_device random_device;
        urng_t urng(random_device());
        std::normal_distribution<> normal(12 t1, 2 t2);

        std::map<long, unsigned> histogram{};
        for (size_t i = 0; i < 10000; i++)
            ++histogram[(size_t)normal(urng)];

        for (const auto [k, v] : histogram)
            if (v / 200 > 0)
                std::cout << std::setw(2) << k << ' '
                          << std::string(v / 200, '*') << '\n';

        return 0;
    }
    ```
]

```bash
 8 **
 9 ****
10 *******
11 *********
12 *********
13 *******
14 ****
15 **
```

==== Exponential distribution #docs("https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution") <exponential>

#note[
    A server receives requests at a rate of 5 requests per minute from each client. You want to rebuild the architecture of the server to make it cheaper. To test if the new architecture can handle the load, its required to build a model of client that sends requests at random intervals with an expected rate of 5 requests per minute.
]

It's easier to simulate the system in seconds (to have more precise measurements). If the client sends 5/min, the rate in seconds should be $lambda = 5 / 60 ~ 0.083$ requests per second.

#figure(caption: `examples/exponential.cpp`)[
    ```cpp
    /* ... */
    int main() {
        std::random_device random_device;
        urng_t urng(random_device());
        std::exponential_distribution<> random_interval(5./60.);

        real_t next_request_time = 0;
        std::vector<real_t> req_per_min = {0};
        for (real_t time_s = 0; time_s < HORIZON; time_s++) {
            if (((size_t)time_s) % 60 == 0)
                req_per_min.push_back(0); t1

            if (time_s < next_request_time)
                continue;

            req_per_min.back()++; t2
            next_request_time = time_s + random_interval(urng);
        }

        real_t mean = 0;
        for (auto x : req_per_min) t3
            mean += x;

        std::cout << mean / req_per_min.size() << std::endl;
    }
    ```
]

The code above has a counter to measure how many requests were sent each minute. A new counter is added every 60 seconds #reft(1) , and it's incremented by 1 each time a request is sent #reft(2) . At the end, the average of the counts is calculated #reft(3) , and it comes out to be about 5 requests every 60 seconds (or 5 requests per minute).

==== Poisson distribution #docs("https://en.cppreference.com/w/cpp/numeric/random/poisson_distribution") <poisson>

The Poisson distribution is closely related to the Exponential distribution, as it randomly generates a number of items in a time unit given the average rate.

```cpp
#include <iomanip>
#include <iostream>
#include <map>
#include <random>

using urng_t = std::default_random_engine;
using real_t = float;
const size_t HORIZON = 10000;

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::poisson_distribution<> poisson(4);

    std::map<long, unsigned> histogram{};
    for (size_t i = 0; i < 10000; i++)
        ++histogram[(size_t)poisson(urng)];

    for (const auto [k, v] : histogram)
        if (v / 100 > 0)
            std::cout << std::setw(2) << k << ' '
                      << std::string(v / 100, '*') << '\n';
}
```

```bash
 0 *
 1 *******
 2 **************
 3 *******************
 4 ********************
 5 ***************
 6 **********
 7 *****
 8 **
 9 *
```
==== Geometric distribution #docs("https://en.cppreference.com/w/cpp/numeric/random/geometric_distribution") <geometric>

A typical geometric distribution, has the same API as the others.

=== Discrete distribution & transition matrices #docs("https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution") <discrete>

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

The `rand_item` instance generates a random integer $x in {0, 1, 2}$ (because 3 items were sepcified in the array #reft(1) , if the items were 10, then $x$ would have been s.t. $0 <= x <= 9$). The ```cpp = {a, b, c}``` syntax can be used to intialize the a discrete distribution because `C++` allows to pass a ```cpp std::array``` to a constructor @std-array.

The ```cpp discrete_distribution``` uses the in the array to generates the probability for each integer. For example, the probability to generate `0` would be calculated as $232 / (232 + 158 + 288)$, the probability to generate `1` would be $158 / (232 + 158 + 288)$ an the probability to generate `2` would be $288 / (232 + 158 + 288)$. This way, the sum of the probabilities is always 1, and the probability is proportional to the weight.

To map the integers to the actual items #reft(2) an ```cpp enum``` is used: for simple enums each entry can be converted automatically to its integer value (and viceversa). In `C++` there is another construct, the ```cpp enum class``` which doesn't allow implicit conversion (the conversion must be done with a function or with ```cpp static_cast```), but it's more typesafe (see @enum).

The ```cpp discrete_distribution``` can also be used for transition matrices, like the one in @rainy-sunny-transition-matrix. It's enough to assign each state a number (e.g. ```cpp sunny = 0, rainy = 1```), and model the transition probability of *each state* as a discrete distribution.

```cpp
std::discrete_distribution[] transition_matrix = {
    /* 0 */ { /* 0 */ 0.8, /* 1 */ 0.2},
    /* 1 */ { /* 0 */ 0.5, /* 1 */ 0.5}
}
```

In the example above the probability to go from state ```cpp 0 (sunny)``` to ```cpp 0 (sunny)``` is 0.8, the probability to go from state ```cpp 0 (sunny)``` to ```cpp 1 (rainy)``` is 0.2 etc...

The `discrete_distribution` can be initialized if the weights aren't already know and must be calculated.

#figure(caption: `practice/2025-01-09/1/main.cpp`)[
    ```cpp
    for (auto &weights t1 : matrix)
        transition_matrix.push_back(
            std::discrete_distribution<>(
                weights.begin(), t2  weights.end() t3 )
        );
    ```
]

The weights are stored in a ```cpp vector``` #reft(1) , and the ```cpp discrete_distribution``` for each state is initialized by indicating the pointer at the beginning #reft(2) and at the end #reft(3) of the vector. This works with dynamic arrays too.

== Data

=== Manual memory allocation

If you allocate with ```cpp new```, you must deallocate with ```cpp delete```, you can't mixup them with ```c malloc()``` and ```c free()```

To avoid manual memory allocation, most of the time it's enough to use the structures in the standard library, like ```cpp std::vector<T>```.

=== Data structures

==== Vectors <std-vector>
// === ```cpp std::vector<T>()``` <std-vector>

You don't have to allocate memory, basically never! You just use the structures that are implemented in the standard library, and most of the time they are enough for our use cases. They are really easy to use.

Vectors can be used as stacks.

==== Deques <std-deque>

// === ```cpp std::deque<T>()``` <std-deque>
Deques are very common, they are like vectors, but can be pushed and popped in both ends, and can b used as queues.

==== Sets <std-set>

Not needed as much, works like the Python set. Can be either a set (ordered) or an unordered set (uses hashes)

==== Maps <std-map>

Could be useful. Can be either a map (ordered) or an unordered map (uses hashes)

== I/O

Input output is very simple in C++.

=== Standard I/O <iostream>

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


== Code structure

=== Classes

- TODO:
    - Maybe constructor
    - Maybe operators? (more like nah)
    - virtual stuff (interfaces)

=== Structs

- basically like classes, but with everything public by default

=== Enums <enum>

- enum vs enum class
- an example maybe
- they are useful enough to model a finite domain

=== Inheritance

#pagebreak()

= Debugging with `gdb`

It's super useful! Trust me, if you learn this everything is way easier (printf won't be useful anymore)

First of all, use the `-ggdb3` flags to compile the code. Remember to not use any optimization like `-O3`... using optimizations makes the program harder to debug.

```makefile
DEBUG_FLAGS := -ggdb3 -Wall -Wextra -pedantic
```

Then it's as easy as running `gdb ./main`

- TODO: could be useful to write a script if too many args
- TODO: just bash code to compile and run
- TODO (just the most useful stuff, technically not enough):
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
// - $p : X times X times U -> X = cal(U)(0, 1) times cal(U)(0, 1)$, the transition probability is a *uniform continuous* distribution #reft(1)
- $p : X times X times U -> X = (cal(U)(0, 1), cal(U)(0, 1)$) the transition probability #reft(1) //the transition probability is a *uniform continuous* distribution #reft(1)
- $g : X -> Y : (r_0, r_1) |-> (r_0, r_1)$ outputs the current state #reft(4)
- $x_0 = (0, 0)$ is the initial state #reft(3)

#figure(caption: `software/1100/main.cpp`)[
    ```cpp
    #include <fstream>
    #include <random>
    #include "../../mocc/mocc.hpp"
    const size_t HORIZON = 10;

    int main() {
        std::random_device random_device;
        urng_t urng(random_device());
        std::uniform_real_distribution<real_t> uniform(0, 1); t1
        std::vector<real_t t2 > state(2, 0); t3
        std::ofstream file("logs");

        for (size_t time = 0; time <= HORIZON; time++) {
            for (auto &r_i : state)
                r_i = uniform(urng); t1

            file << time << ' ';
            for (auto r_i : state)
                file << r_i << ' '; t4
            file << std::endl;
        }

        file.close(); return 0;
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
    /* ... */
    const size_t HORIZON = 100;
    struct MDP { real_t state[2]; };

    int main() {
        /* ... */
        std::vector<MDP> mdps(2, {0, 0});

        for (size_t time = 0; time <= HORIZON; time++)
            for (size_t r = 0; r < 2; r++) {
                mdps[0].state[r] =
                    mdps[1].state[r] * uniform(urng);
                mdps[1].state[r] =
                    mdps[0].state[r] + uniform(urng);
            }

        /* ... */
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

$ p((x_0 ', x_1 ')|(x_0, x_1), (u_0, u_1)) = cases(1 & "if" ... \ 0 & "otherwise") $

The implementation would be

#figure(caption: `software/1300/main.cpp`)[
    ```cpp
    /* ... */

    int main() {
        /* ... */
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

            /* ... */
        }

        /* ... */
    }
    ```
] <mdps-connection-3>

=== MDPs network pt.3 `[1400]`

The original model behaves exactly lik @mdps-connection-3, with a different implementation. As an exercise, the reader is encouraged to come up with a different implementation for @mdps-connection-3.

#pagebreak()

== Traffic light `[2000]`

This example models a *traffic light*. The three original versions (`2100`, `2200` and `2300`) have the same behaviour, with a different implementation. The one reported here behaves like the original versions, with a simpler implementation. Let $T$ be the MDP that describes the traffic light, s.t.
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

Meaning that, if the input is $epsilon$, $T$ maintains the same color with probability 1. Otherwise, if the input is $sigma$, $T$ changes color with probability 1, iif the transition is valid (green $->$ yellow, yellow $->$ red, red $->$ green).

#figure(caption: `software/2000/main.cpp`)[
    ```cpp
    #include <fstream>
    #include <random>
    #include "../../mocc/mocc.hpp"
    const size_t HORIZON = 1000;
    enum Light { GREEN = 0, YELLOW = 1, RED = 2 }; t1

    int main() {
        std::random_device random_device;
        urng_t urng(rand_device());
        std::uniform_int_distribution<>rand_interval(60, 120); t2
        Light traffic_light = Light::RED;
        size_t next_switch = rand_interval(urng);
        std::ofstream file("logs");

        for (size_t time = 0; time <= HORIZON; time++) {
            file << time << ' ' << next_switch - time << ' '
                 << traffic_light t3 << std::endl;
            if (time < next_switch) continue;
            traffic_light = t4
                (traffic_light == RED
                     ? GREEN
                     : (traffic_light == GREEN ? YELLOW : RED));

            next_switch = time + rand_interval(urng);
        }
        file.close(); return 0;
    }
    ```
]

To reperesent the colors the cleanest way is to use an ```cpp enum```. C++ has two types of enums: simple ```cpp enum```s and ```cpp enum class```es. In this example a *simple* ```cpp enum``` is good enough #reft(1), because its constants are automatically casted to their value when mapped to string #reft(3); this doesn't happen with ```cpp enum class```es because they are stricter types, and require explicit casting.

The behaviour of the formula described above is implemented with a couple ternary operators #reft(4).

// TODO:
//   - #reft(1) ```cpp enum``` vs ```cpp enum class```
//   - #reft(2) reference the same trick used in the uniform int distribution example
//   - #reft(3) enum is easier to use, because it just logs the integer value associated with the constant
//   - #reft(4) is basically the behaviour of the formula described above
//   - how is the time rapresented?
//   - how can it be implemented with ```cpp mocc```?


== Control center

This example adds complexity to the traffic light by introducing a *remote control center*, network faults and repairs. Having many communicating components, this example requires more structure.

=== No network `[3100]`

The first step into building a complex system is to model it's components as units that can communicate with eachother. The traffic light needs to be to re-implemented as a component (which can be easily done with the ```c mocc``` library).

#figure(caption: `software/3100/parameters.hpp`)[
    ```cpp
    #pragma once t1

    #include "../../mocc/mocc.hpp"
    #include <cstddef>
    #include <random>

    enum Light { GREEN = 0, YELLOW = 1, RED = 2 }; t2
    const size_t HORIZON = 1000; t3
    static std::random_device random_device; t4
    static urng_t urng = urng_t(random_device()); t4
    ```
]

The simulation requires some global variables and types in order to work, the simplest solution is to make a header file with all these data:
- ```cpp #pragma once``` #reft(1) is used instead of ```cpp #ifndef xxxx #define xxxx```; it has the same behaviour (preventing multiple definitions when a file is imported multiple times)... technically ```cpp #pragma once``` isn't part of the standard, yet all modern compilers support it
- ```cpp enum Light``` #reft(2) has a more important job in this example: it's used to communicate values from the *controller* to the *traffic light* via the *network*; technically it could be defined in its own file, but, for the sake of the example, it's not worth to make code navigation more complex
- there is no problem in defining global constants #reft(3), but global variables are generally discouraged #reft(4) (the alternative would be a singleton or passing the values as parameters to each component, but it would make the example more complex than necessary)

#figure(caption: `software/3100/traffic_light.hpp`)[
    ```cpp
    #pragma once

    #include "../../mocc/system.hpp"
    #include "../../mocc/time.hpp"
    #include "parameters.hpp"

    class TrafficLight : public Timed t1 {
        std::uniform_int_distribution<> random_interval; t2
        Light l = Light::RED; t3

      public:
        TrafficLight(System *system)
            : random_interval(60, 120),
              Timed(system, 90, TimerMode::Once) {} t4

        void update(U) override { t5
            l = (l == RED ? GREEN : (l == GREEN ? YELLOW : RED));
            timer.set_duration(random_interval(urng)); t7
        }

        Light light() { return l; } t8
    };
    ```
]

By using the ```cpp mocc``` library, the re-implementation of the traffic light is quite simple. A ```cpp TrafficLight``` is a ```cpp Timed``` component #reft(1), which means that it has a `timer`, and whenever the `timer` reaches 0 it #reft(5) it receives a notification (the method ```cpp update(U)``` is called, and the traffic light switches color). The `timer` needs to be attached to a ```cpp System``` for it to work #reft(4), and must be initialized. In the library there are two types of ```cpp Timer```
- ```cpp TimerMode::Once```: when the timer ends, it doesn't automatically restart (it must be manually reset, this allows to set a different after each time the timer reaches 0, e.g. with a random variable #reft(2) #reft(7))
- ```cpp TimerMode::Repeating```: the ```cpp Timer``` automatically resets with the last value set
Like before, the state of the MDP is just the ```cpp Light``` #reft(3), which can be read #reft(8) but not modified by external code.

#figure(caption: `software/3100/main.cpp`)[
    ```cpp
    #include <fstream>

    #include "parameters.hpp"
    #include "traffic_light.hpp"

    int main() {
        std::ofstream file("logs");

        System system; t1
        Stopwatch stopwatch; t2
        TrafficLight traffic_light(&system); t3
        system.addObserver(&stopwatch);

        while (stopwatch.elapsed() <= HORIZON) {
            file << stopwatch.elapsed() << ' '
                 << traffic_light.light() << std::endl;
            system.next(); t4
        }

        file.close();
        return 0;
    }
    ```
]

The last step is to put together the system and run it. A ```cpp System``` #reft(1) is a simple MDP which sends an output $epsilon$ when the ```cpp next()``` method is called. By connecting all the components to the ```cpp System``` it's enough to repeatedly call the ```cpp next()``` method to simulate the whole system.

A ```cpp Stopwatch``` #reft(2) is needed to measure how much time has passed since the simulation started, and the ```cpp TrafficLight``` #reft(3) is connected to a ```cpp timer``` which itself is connected to the ```cpp System```.

=== Network monitor

The next objective is to introduce a control center which sends information to the traffic light via a network. The traffic light just takes the value it receives via network and displays it.

==== No faults `[3200]`

#figure(caption: `software/3200/control_center.hpp`)[
    ```cpp
    /* ... */

    class ControlCenter
        : public Timed, public Notifier<Payload> t1 {
        std::uniform_int_distribution<> random_interval;
        Light l = Light::RED;

      public:
        ControlCenter(System *system)
            : random_interval(60, 120),
              Timed(system, 90, TimerMode::Once) {}

        void update(U) override {
            l = (l == RED ? GREEN : (l == GREEN ? YELLOW : RED));
            notify(l); t1
            timer.set_duration(random_interval(urng));
        }

        Light light() { return l; }
    };
    ```
]

The ```cpp ControlCenter``` has the same behaviour the traffic light had before, with a small difference: it notifies #reft(1) other components when the light switches. The type of the notification is ```cpp Payload``` (which is just a ```cpp STRONG_ALIAS``` for ```cpp Light```), this way only components that take a ```cpp Payload``` (i.e. the ```cpp Network``` component) can be connected to the ```cpp ControlCenter```.

#figure(caption: `software/3200/traffic_light.hpp`)[
    ```cpp
    /* ... */

    class TrafficLight : public Observer<Message>,
                         public Notifier<Light> {
        Light l = Light::RED;

      public:
        void update(Message message) override {
            t2 notify(l = message t1);
        }

        Light light() { return l; }
    };
    ```
]

At this point the traffic light is easier to implement, as it just takes in input a ```cpp Message``` from other components (i.e. the ```cpp Network```), changes its light #reft(1) and notifies other components #reft(2) of the change (```cpp Message``` is just a ```cpp STRONG_ALIAS``` for ```cpp Light```).

#figure(caption: `software/3200/parameters.hpp`)[
    ```cpp
    /* ... */
    #include "../../mocc/alias.hpp"

    /* ... */
    STRONG_ALIAS(Payload, Light);
    STRONG_ALIAS(Message, Light);
    ```
]

The ```cpp STRONG_ALIAS```es are defined in the `parameters.hpp` file (it's enough to import the `mocc/alias.hpp` file from the library). Strong aliases are different from ```cpp typedef``` or ```cpp using``` aliases, as the new type is different from the type it aliases (```cpp Payload``` is a different type from ```cpp Light```), but their values can be exchanged (a ```cpp Light``` value can be assigned to a ```cpp Payload``` and viceversa). Aliases enable type-safe connections among components.

#figure(caption: `software/3200/network.hpp`)[
    ```cpp
    /* ... */

    class Network : public Timed,
                    public Buffer<Payload>, t1
                    public Notifier<Message> {

      public:
        Network(System *system)
            : Timed(system, 0, TimerMode::Once) {}

        void update(Payload payload) override {
            if (buffer.empty())
                timer.set_duration(2);
            Buffer<Payload>::update(payload);
        }

        void update(U) override {
            if (!buffer.empty()) {
                notify((Light)buffer.front());
                buffer.pop_front();
                if (!buffer.empty())
                    timer.set_duration(2);
            }
        }
    };
    ```
]

The simplest form of network has an illimited ```cpp Buffer``` #reft(1) for the incoming messages, and every 2 seconds it sends the message to the destination (to simulate a delay). This model of the network has many problems: it doesn't account for faults (messages are corrupted / lost), buffer overflow, the fact that all messages take the same time to be sent etc...


#figure(caption: `software/3200/traffic_light.hpp`)[
    ```cpp
    /* ... */

    class Monitor : public Recorder<Payload>,
                    public Recorder<Light>,
                    public Observer<> {

        int time = 0; bool messages_lost = false;

      public:
        Monitor() : Recorder<Payload>(RED), Recorder<Light>(RED) {}

        void update() override {
            if (Recorder<Payload>::record !=
                Recorder<Light>::record)
                time++;
            else
                time = 0;

            if (time > 3)
                messages_lost = true;
        }

        bool is_valid() { return !messages_lost; }
    };
    ```
]

The ```cpp Monitor``` is a component that takes inputs from both the ```cpp ControlCenter``` and the ```cpp TrafficLight``` and checks if messages are lost (a message is lost if it takes more then 3 seconds for the traffic light to change).

==== Faults & no repair `[3300]`

#figure(caption: `software/3300/network.hpp`)[
    ```cpp
    /* ... */

    class Network : public Timed,
                    public Buffer<Payload>,
                    public Notifier<Message> {
        std::bernoulli_distribution random_fault; t1

      public:
        /* ... */

        void update(U) override {
            if (!buffer.empty()) {
                if (!random_fault(urng)) t2
                    notify((Light)buffer.front());
                buffer.pop_front();
                if (!buffer.empty())
                    timer.set_duration(2);
            }
        }
    };
    ```
]

The first change is to add faults to the network #reft(1), which can be done easily by using a ```cpp std::bernoulli_distribution``` with a certain fault probability (e.g. 0.01), and send the message only if there is no fault. Once the message is lost nothing can be done, the system doesn't recover.

#pagebreak()

=== Faults & repair `[3400]`

#figure(caption: `software/3400/network.hpp`)[
    ```cpp
    /* ... */

    class Network : public Timed,
                    public Buffer<Payload>,
                    public Notifier<Message> {
        std::bernoulli_distribution random_fault;
        std::bernoulli_distribution random_repair; t1

      public:
        /* ... */

        void update(U) override {
            if (!buffer.empty()) {
                if (!random_fault(urng)||random_repair(urng)) t2
                    notify((Light)buffer.front());
                buffer.pop_front();
                if (!buffer.empty())
                    timer.set_duration(2);
            }
        }
    };
    ```
]

The next idea is to add repairs #reft(1) when the system fails. In this case the repairs are random for simplicity #reft(2), but there are smarter ways to handle a network fault.

=== Faults & repair + correct protocol `[3500]`

#figure(caption: `software/3500/network.hpp`)[
    ```cpp
    /* ... */

    class Network : public Timed,
                    public Buffer<Payload>,
                    public Notifier<Message>,
                    public Notifier<Fault> t1 {
      public:
        /* ... */

        void update(U) override {
            if (!buffer.empty()) {
                if (random_fault(urng)) {
                    if (random_repair(urng))
                        Notifier<Message>::notify(
                            (Light)buffer.front());
                    else
                        Notifier<Fault>::notify(true); t2
                } else {
                    Notifier<Message>::notify(
                        (Light)buffer.front());
                }

                buffer.pop_front();
                if (!buffer.empty())
                    timer.set_duration(2);
            }
        }
    };
    ```
]

In the last version, the network sends a notification #reft(2) when there is a ```cpp Fault``` #reft(1) (which is just a ```cpp STRONG_ALIAS``` for ```cpp bool```), this way the ```cpp TrafficLight``` can recover in case of errors.

#figure(caption: `software/3500/traffic_light.hpp`)[
    ```cpp
    /* ... */

    class TrafficLight : public Observer<Fault>, t1
                         public Observer<Message>,
                         public Notifier<Light> {
        /* ... */

        void update(Fault) override {
            l = Light::RED;
            notify(l);
        }
    };
    ```
]

When the ```cpp TrafficLight``` detects a ```cpp Fault``` it turns to ```cpp Light::RED``` for safety reasons.

#figure(caption: [connections in `software/3500/main.cpp`])[
    ```cpp
    System system;
    Monitor monitor;
    Network network(&system);
    Stopwatch stopwatch;
    TrafficLight traffic_light;
    ControlCenter control_center(&system);

    system.addObserver(&monitor);
    system.addObserver(&stopwatch);
    network.Notifier<Message>::addObserver(&traffic_light);
    network.Notifier<Fault>::addObserver(&traffic_light);
    traffic_light.addObserver(&monitor);
    control_center.addObserver(&network);
    control_center.addObserver(&monitor);
    ```
]

== Statistics

=== Expected value `[4100]`

In this example the goal is to simulate a development process (phase 0, phase 1, and phase 2), and calculate the cost of each simulation.


#figure(caption: `software/4100/main.cpp`)[
    ```cpp
    #include <cstddef>
    #include <fstream>
    #include <random>

    #include "../../mocc/mocc.hpp"

    int main() {
        std::random_device random_device;
        urng_t urng(random_device());

        std::vector<std::discrete_distribution<>>
            transition_matrix = {
                {0, 1},
                {0, .3, .7},
                {0, 0, .2, .8},
                {0, .1, .1, .1, .7},
                {0, 0, 0, 0, 1},
            };

        real_t time = 0;
        size_t phase = 0, costs = 0;

        std::ofstream file("logs");

        while (phase != 4) {
            time++;
            if (phase == 1 || phase == 3)
                costs += 20;
            else if (phase == 2)
                costs += 40;

            phase = transition_matrix[phase](urng);
            file << time << ' ' << phase << ' ' << costs
                 << std::endl;
        }

        file.close();
        return 0;
    }
    ```
]

=== Probability `[4200]`

This example behaves like the previous one, but uses the Monte Carlo method @monte-carlo-method to calculate the probability the cost is less than a certain value

// In this one we simulate a more complex software developmen process, and we calculate the average cost (Wait, what? Do we simulate it multiple times?)

#figure(caption: `software/4200/main.cpp`)[
    ```cpp
    /* ... */

    const size_t ITERATIONS = 10000;

    int main() {
        Stat cost_stat;
        size_t less_than_100_count = 0;
        real_t time = 0;

        std::ofstream file("logs");

        for (int iter = 0; iter < ITERATIONS; iter++) {
            size_t phase = 0, costs = 0;

            while (phase != 4) {
                time++;
                if (phase == 1 || phase == 3)
                    costs += 20;
                else if (phase == 2)
                    costs += 40;

                phase = transition_matrix[phase](urng);
                file << time << ' ' << phase << ' ' << costs
                     << std::endl;
            }

            cost_stat.save(costs);
            if (costs < 100)
                less_than_100_count++;
        }

        std::cout << cost_stat.mean() << ' ' << cost_stat.stddev()
                  << ' '
                  << (double)less_than_100_count / ITERATIONS
                  << std::endl;

        file.close();
        return 0;
    }
    ```
]

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
    caption: "A simple Markov Chain",
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

Model CheCking library for the exam

== Design

Basically: the "Observer Pattern" @observer-pattern can be used to implement MDPs, because a MDP is like an entity that "is notified" when something happens (receives an input, in fact, in the case of MDPs, another name for input is "action"), and notifies other entities (gives an output, or reward).

#figure(caption: `https://refactoring.guru/design-patterns/observer`)[
    #block(width: 100%, inset: 1em, stroke: luma(245), fill: luma(254), image("public/observer.png"))
]

By using the generics (templates) in `C++` it's possible to model type-safe MDPs, whose connections are easier to handle (if an entity receives inputs of type ```cpp Request```, it cannot be mistakenly connected to an entity that gives an output of type ```cpp Time```).

#pagebreak()

== ```cpp mocc```

```cpp
using real_t = double;
```

The ```cpp real_t``` type is used as an alias for floating point numbers to ensure the same format is used everywhere in the library.

```cpp
using urng_t = std::default_random_engine;
```

The ```cpp urng_t``` type is used as an alias for ```cpp std::default_random_engine``` to make the code easier to write.

== ```cpp math```

```cpp
class Stat
```

The ```cpp Stat``` class is used to calculate the mean and the standard deviation of a set of values (as discussed in @incremental-average and @welford)

#block(inset: (left: 1em))[
    ```cpp
    void save(real_t x);
    ```

    The ```cpp save()``` method is used to add a value to the set of values. The mean and the standard deviation are automatically updated when a new value is saved.

    ```cpp
    real_t mean() const;
    ```

    Returns the precalculated mean.

    ```cpp
    real_t stddev() const;
    ```

    Returns the precalculated standard deviation.

    #heading(level: 3, outlined: false, numbering: none)[Example]

    ```cpp
    Stat cost_stat;

    cost_stat.save(302);
    cost_stat.save(305);
    cost_stat.save(295);
    cost_stat.save(298);

    std::cout
      << cost_stat.mean() << " "
      << cost_stat.stddev() << std::endl;
    ```
]

#pagebreak()

== ```cpp time```

```cpp
STRONG_ALIAS(T, real_t)
```

The ```class T``` is the type for the *time*, it's reperesented as a ```cpp real_t``` to allow working in smaller units of time (for exapmle, when the main unit of time of the simulation is the _minute_, it could still be useful to work with _seconds_). ```class T``` is a *strong alias*, meaning that if a MDP takes in input ```cpp T```, it cannot be connected to a MDP that gives in output a simple ```cpp real_t```.

```cpp
class Stopwatch : public Observer<>, public Notifier<T>
```

A ```cpp Stopwatch``` starts at time ```cpp 0```, and each iteration of the system it increments it's time counter by $Delta$. It can be used to measure time from a certain point of the simulation (it can be at any point of the simulation). It sends a notification with the elapsed time at each iteration.

#block(inset: (left: 1em))[
    ```cpp
    Stopwatch(real_t delta = 1);
    ```

    The default $Delta$ for the ```cpp Stopwatch``` is ```cpp 1```, but it can be changed. Usually, a ```cpp Stopwatch``` is connected to a ```cpp System```.

    ```cpp
    real_t elapsed();
    ```

    Returns the time elapsed since the ```cpp Stopwatch``` was started.

    ```cpp
    void update() override;
    ```

    This method *must* be called to update the ```cpp Stopwatch```. It is automatically called when the ```cpp Stopwatch``` is connected to a ```cpp System```, or, more generally, to a ```cpp Notifier<>```.

    #heading(level: 3, outlined: false, numbering: none)[Example]

    ```cpp
    System system;
    Stopwatch s1, s2(2.5);

    size_t iteration = 0;
    system.addObserver(&s1);

    while (s1.elapsed() < 10000) {
        if (iteration == 1000) system.addObserver(&s2);
        system.next(); iteration++;
    }

    std::cout << s1.elapsed() <<' '<< s2.elapsed() << std::endl;
    ```
]

```cpp
enum class TimerMode { Once, Repeating }
```

A ```cpp Timer``` can be either in ```cpp Repeating``` mode or in ```cpp Once``` mode:
- In ```cpp Repeating``` mode, everytime the timer hits 0, it resets
- In ```cpp Once``` mode, when the timer hits 0, it stops

```cpp
class Timer : public Observer<>, public Notifier<>
```

A ```cpp Timer``` starts with a certain duration. At every iteration the duration decreases by $Delta$. When a ```cpp Timer``` hits 0, it sends a notification to its subscribers (with no input value).

#block(inset: (left: 1em))[

    ```cpp
    Timer(real_t duration, TimerMode mode, real_t delta = 1);
    ```

    A ```cpp Timer``` requires the starting duration and it's mode. It's more useful to use the ```cpp Once``` mode if the duration is different at each reset, this way it can be set manually.

    ```cpp
    void set_duration(real_t time);
    ```

    Sets the current duration of the ```cpp Timer```. It's useful when the duration is generated randomly each time the ```cpp Timer``` hits 0.

    ```cpp
    void update() override;
    ```

    This method must be called to updated the time of the ```cpp Timer```. Generally the ```cpp Timer``` is connected to a ```cpp System```.

    #heading(level: 3, outlined: false, numbering: none)[Example]

    ```cpp
    TODO: example
    ```
]

#pagebreak()

== ```cpp alias```

```cpp
template <typename T> class Alias
```

The ```cpp class Alias``` is used to create *strong aliases* (a strong alias is a type that can be used in place of its underlying type, except in templates, as its considere a totally different type).

#block(inset: (left: 1em))[
    ```cpp
    Alias() {}
    ```

    It initialized the value for the underlying type to it's default one.

    ```cpp
    Alias(T value)
    ```

    It initialized the underlying type with a certain value. Useful when the underlying type needs complex initialization. It also allows to assign a value of the underlying type (e.g. ```cpp Alias<int> a_int = 5;```)

    ```cpp
    operator T() const
    ```

    Allows the ```cpp Alias<T>``` to be casted to ```cpp T``` (e.g. ```cpp Alias<int> a_int = 5; int v = (int)a_int;```). The casting doesn't need to be explicit.
]

```cpp
STRONG_ALIAS(ALIAS, TYPE)
```

The ```cpp STRONG_ALIAS``` macro is used to quickly create a strong alias. The ```cpp Alias<T>``` class is never used directly.

== ```cpp observer```

```cpp
template <typename... T> class Observer
```

- TODO

== ```cpp notifier```

```cpp
template <typename... T> class Notifier
```

- TODO

== Auxiliary

```cpp
template <typename T> class Recorder : public Observer<T>
```

```cpp
class Client : public Observer<U...>,
               public Notifier<Observer<U...> *, T>
```

- TODO (+ ```cpp using Host```)

```cpp
class Server : public Observer<Observer<U...> *, T>
```

- TODO (+ ```cpp using Host```)

```cpp
class System : public Notifier<>
```

- TODO

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

    STRONG_ALIAS(ProjInit, real_t) t3
    STRONG_ALIAS(TaskDone, real_t) t3
    STRONG_ALIAS(EmplCost, real_t) t3

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

```cpp
System system; t1
Stopwatch stopwatch; t2

system.addObserver(&stopwatch); t3

while (stopwatch.elapsed() <= HORIZON) t4
    system.next(); t5
```

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

// == UI testing?
//
// === Playwright

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
