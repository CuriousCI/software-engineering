#set text(font: "New Computer Modern", lang: "en", weight: "light", size: 11pt)
#set page(margin: 1.75in)
#set par(leading: 0.55em, spacing: 0.85em, first-line-indent: 1.8em, justify: true)
#set heading(numbering: "1.1")
#set math.equation(numbering: "(1)")

#show heading: set block(above: 1.4em, below: 1em, sticky: false)
#show sym.emptyset : sym.diameter 
#show raw.where(block: true): block.with(inset: 1em, stroke: (y: (thickness: .1pt, dash: "dashed")))

#let note(body) = block(inset: 1em, stroke: (thickness: .1pt, dash: "dashed"), [*Note*: \ #body])

// #set list(spacing: 0.85em)
// #set par(leading: 0.55em, spacing: 0.55em, first-line-indent: 1.8em, justify: true)
// #show par: set block(spacing: 5pt)

#show outline.entry.where(level: 1): it => {
  show repeat : none
  v(1.1em, weak: true)
  text(size: 1em, strong(it))
}

#show raw: r => {
  show regex("t(\d)"): it => {
    box(baseline: .2em, circle(stroke: .5pt, inset: .1em, align(center + horizon, 
          text(size: .8em, strong(repr(it).slice(2, -1)))
    )))
  }

  r
}

#let reft(reft) = box(height: 1em, clip: true, baseline: 0.2em, circle(stroke: .5pt, inset: 0.1em, 
  align(center + horizon, text(font: "CaskaydiaCove NF", size: 0.7em, strong(str(reft))))
))

#page(align(center + horizon)[
    #heading(outlined: false, numbering: none, text(size: 1.5em)[Software Engineering]) 
    #text(size: 1.3em)[Cicio Ionuț]
    #align(bottom, datetime.today().display("[day]/[month]/[year]"))
  ]
)

#page(outline(indent: auto))

= Software models

When designing *complex software* we have to make major *design choices* at the beginning of the project. Often those choices can't be driven by experience or reasoning alone, that's why a *model* of the project is needed to compare different solutions. Our formal tool of choice is the *Discrete Time Markov Chain* (@dtmc).

== The "Amazon Prime Video" article 

If you were tasked with designing the software architecture for *Amazon Prime Video* _(a live streaming service for Amazon)_, how would you go about it? What if you had the *non-functional requirement* to keep the costs as low as possible?

In a recent article, Marcin Kolny, a Senior SDE at Prime Video, describes how they _"*reduced the cost* of the audio/video monitoring infrastructure by 90%"_ @primevideo2024 by using a monolith application instead of distributed microservices (an outcome one wouldn't usually expect). 

While there isn't always definitive answer, one way to go about this kind choice is building a model of the system to compare the solutions. In the case of Prime Video, _"the audio/video monitoring service consists of three major components:"_ @primevideo2024
- the *media converter* converts input audio/video streams 
//to frames or decrypted audio buffers that are sent to detectors
- the *defect detectors* execute algorithms that analyze frames and audio buffers in real-time looking for defects and send notifications
- the *orchestrator* controls the flow in the service

#align(center)[
  #figure({
    set text(font: "CaskaydiaCove NF", weight: "light", lang: "en")
    image("public/audio-video-monitor.svg", width: 92%)
  }, caption: "Model of the audio/video monitoring system") <prime-video>
]

We want to model the components as some kind of stateful machine (with inputs and outputs, @prime-video), interconnect them and *simulate* the behaviour of the system as a whole.

 // real-time notifications whenever a defect is found
// #lorem(100)

// \

// #block(fill: red, width: 100%, height: auto)[#lorem(1000)]

// Our service consists of three major components. .  
// (such as video freeze, block corruption, or audio/video synchronization problems) and 

// @primevideo2024 

// #align(center)[
//   #figure({
//     set text(font: "CaskaydiaCove NF", weight: "light", lang: "en")
//     // image("public/weather-driving-markov-chains.svg", width: 80%)
//   }, caption: "the 'traffic' system modeled with 2 subsystems") <traffic> 
// ]

== Formal notation <traffic>

=== The concept of time

TODO: rewrite

The models treated in the course evolve through *time*. Time can be modeled in many ways (I guess?), but, for the sake of simplicity, we will consider discrete time. Let $W$ be the _'weather system'_ and $D$ the _'driving ability' system _ in @traffic, we can define the evolution of $D$ as 

\

$ 
  & D(0) = "'good'" \
  & D(t + d) = f(D(t), W(t)) 
$

\

Given a time instant $t$ (let's suppose 12:32) and a time interval $d$ (1 minute), the driving ability of $D$ at 12:33 depends on the driving ability of $D$ at the time 12:32 and the weather at 12:32.

=== Markov Chain

A Markov Chain is...

=== DTMC (Discrete Time Markov Chain) <dtmc>

A DTMC $M$ is a tuple $(U, X, Y, p, g)$ s.t.
- $U != emptyset and X != emptyset and Y != emptyset$ _(otherwise stuff doesn't work)_
- $U$ can be either 
  - ${ u_1, ..., u_n }$ where $u_i$ is an input value 
  - ${()}$ if $M$ doesn't take any input
- $X = { x_1, ..., x_n }$ where $x_i$ is a state
- $Y = { y_1, ..., y_n }$ where $y_i$ is an output value
- $p : X times X times U -> [0, 1]$ is the transition function
- $g : X -> Y$ is the output function

\

$
  forall x in X space.en forall u in U space.en sum_(x' in X) p(x'|x, u) = 1
$

\

$
  & M(0) = x_1 \
  & M(t + d) = cases(
    x_1 quad & "with probability " p(x_1|M(t), U(t)) \
    x_2 quad & "with probability " p(x_2|M(t), U(t)) \
    ...
  )
$

\

It's interesting to notice that the transition function depends on the input values. If you consider the _'driving ability'_ system in @traffic, you can see that the probability to go from `good` to `bad` is higher if the weather is rainy and lower if it's sunny. 

#pagebreak()

==== An example of DTMC

Let's consider the development process of a team. We can define a DTMC $M = (U, X, Y, p, g)$ s.t.
- $U = {()}$, as it doesn't have any input
- $X = {0, 1, 2, 3}$ 
- $Y = "Cost" times "Duration (in months)"$

#align(center)[
  #figure({
    set text(font: "CaskaydiaCove NF", weight: "light", lang: "en")
    image("public/development-process-markov-chain.svg")
  }, caption: "the model of a team's development process") <development-process> 
]

\

$
  g(x) = cases(
    (0, 0) & quad "if " x = 0 \
    (20000, 2) & quad "if " x = 1 \
    (40000, 4) & quad "if " x = 2 \
    (20000, 2) & quad "if " x = 3
  )
$

=== Network of Markov Chains

TODO...

==  Tips and tricks

=== Mean

TODO: 'mean' trick, ggwp

$
  epsilon_n = (sum_(i = 0)^n v_i) / n \
  epsilon_(n + 1) = (sum_(i = 0)^(n + 1) v_i) / (n + 1) = 
  = ((sum_(i = 0)^(n) v_i) + v_(n + 1)) / (n + 1) = 
  (sum_(i = 0)^(n) v_i) / (n + 1) + v_(n + 1) / (n + 1) = \
  ((sum_(i = 0)^(n) v_i) n) / ((n + 1) n) + v_(n + 1) / (n + 1) = 
  (sum_(i = 0)^(n) v_i) / n dot.c n / (n + 1) + v_(n + 1) / (n + 1) = 
  (epsilon_n dot n + v_(n + 1)) / (n + 1)
$

=== Eulero's method for differential equations

Useful later...

#pagebreak()

= `C++`

This section will cover the basics needed for the exam.

== Useful ```cpp #include <random>``` features not in `C` <random>

The ```cpp <random>``` library offers useful tools to build our models. It makes Markov Chains and probabilistic models really easy to implement.

// Randomness is fundamental in our models. There are key differences that make ```cpp <random>``` more ergonomic to use in `C++`
// Even if you decide to not use any of the `C++` fancy features, t

=== ```cpp random()``` vs ```cpp random_device``` vs ```cpp default_random_engine``` <random_engine>

In `C++` there are many ways to *generate random numbers*, I'm gonna keep it short and sweet: don't use ```cpp random()```, use ```cpp std::random_device``` to generate the *seed* and from then on just some random engine like ```cpp std::default_random_engine```.

#figure(caption: `examples/random.cpp`)[
```cpp
#include <iostream>
#include <random>

int main() {
    std::cout << random() t1 << std::endl;

    std::random_device random_device; t2
    std::cout << random_device() t3 << std::endl;

    std::default_random_engine r_engine(random_device() t4 ); 
    std::cout << r_engine() t5 << std::endl;
}
```
] <random>

I'll explain: ```cpp random()``` #reft(1) doesn't work very well (TODO: link to the article\*), you're encouraged to use the generators `C++` offers... but each works in a different way, and you have to choose the best one depending on your needs. ```cpp std::random_device``` #reft(2) is used to generate the *seed* #reft(4) because it uses device randomness (if available, TODO: link to docs / article \*), but it's really slow. That *seed* is used to to instantiate one of the other engines #reft(5), like ```std::default_random_engine``` (TODO: link with different engines and types). TODO BONUS: only r_eng used with distr

#note[
  If you see #reft(3) and #reft(5), random_device and r_engine aren't functions, they are instances, but you can call the ```cpp ()``` operator on them because C++ has operator overloading, and it allows you to call custom operators on instances... the same goes for ```cpp std::cout``` and ```cpp <<```
]

=== Distributions <distributions>

Just the capability of generating random numbers isn't enough, we often need to manipulate those numbers to fit our needs. Luckly, `C++` covers basically all of them... for example, this is how easy it is to simulate a transition matrix in @development-process:

#figure(caption: `examples/transition_matrix.cpp`)[
```cpp
#include <iostream>
#include <random>

const size_t HORIZON = 15;

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());

    std::discrete_distribution<size_t> transition_matrix[] = {
        {0, 1},
        {0, .3, .7},
        {0, .2, .2, .6},
        {0, .1, .2, .1, .6},
        {1},
    };

    size_t state = 0;
    for (size_t time = 0; time < HORIZON; time++) {
        state = transition_matrix[state](random_engine);
        std::cout << state << std::endl;
    }
}
```
] <transition-matrix>

==== ```cpp std::uniform_int_distribution``` <uniform-int>

==== ```cpp std::uniform_real_distribution``` <uniform-real>

==== ```cpp std::bernoulli_distribution``` <bernoulli>

==== ```cpp std::poisson_distribution``` <poisson>

==== ```cpp std::geometric_distribution``` <geometric>

==== ```cpp std::discrete_distribution``` <discrete>

== Dynamic structures 

=== ```cpp std::vector<T>()``` instead of ```cpp malloc()``` <vector>

=== ```cpp std::deque<T>()```

=== Sets

=== Maps

== I/O

=== ```cpp #include <iostream>``` <iostream>

=== Files <files>

#pagebreak()

= Exercises 

Each exercise has 4 digits `xxxx` that are the same as the ones in the `software` folder in the course material.

== `[1000]` First examples 

Now we have to put together our *formal definitions* and our `C++` knowledge to build some simple DTMCs and networks.

=== `[1100]` A simple Markov Chain 


Let's begin our modeling journey by implementing a DTMC $M$ s.t.
- $U = {()}$ it takes no input
- $X = [0,1] times [0,1]$ it has infinite states (all the pairs of real numbers between 0 and 1)
- $Y = [0,1] times [0,1]$
- $p : X times X times U -> X = cal(U)(0, 1) times cal(U)(0, 1)$ 
- $g : X -> Y : (r_0, r_1) |-> (r_0, r_1)$ it outputs the current state

- $X(0) = (0, 0)$

\

#figure(```cpp
#include <fstream>
#include <random>

using real_t = double;

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_real_distribution<real_t> uniform(0, 1); t1

    const size_t HORIZON = 10; t2
    std::vector<real_t> state(2, 0); t3

    for (size_t time = 0; time <= HORIZON; time++)
        for (auto &r : state)
            r = uniform(random_engine); t4

    return 0;
}
```, caption: `software/1100/main.cpp`)

\

=== `[1200]` Connect Markov Chains pt.1 

In this exercise we build 2 markov chains, and connect them...

Now let's model a system with two DTMCs $M_0, M_1$, and lets define the functions

$U(M_0, t + 1) = dots.c$
$U(M_1, t + 1) = dots.c$

=== `[1300]` Connect Markov Chains pt.2 

The same as above, but with a different connection

=== `[1400]` Connect Markov Chains pt.3 

The same as above, but with a different connection

== `[2100, 2200, 2300]` Traffic light 

This is 

== `[3000]` Control center 

=== `[3100]` No network 

=== `[3200]` Network monitor (no faults) 

=== `[3300]` Network monitor (faults, no repair) 

=== `[3400]` Network monitor (faults, repair) 

=== `[3500]` Network monitor (faults, repair, correct protocol) 

== `[4000]` Statistics 

=== `[4100]` Expected value 

=== `[4200]` Probability 

== `[5000]` Transition matrix

== `[6000]` Complex systems

=== `[6100]` Insulin pump

=== `[6200]` Buffer 

=== `[6300]` Server

= Exam

== Development team (time & cost)

== Backend load balancing

== Heater simulation

= CASE library

TODO...


#page(bibliography("bibliography.bib"))

// The evolution of the  $d$... for examples, if we have a system $S$, we say that $S(t)$ is the state of the system at time $t$, and $S(t + d) = f(S(t), x, y, z)$

// Let's define $TT_d$ as the set of time 
// $ TT_d = { t * d | t in NN } $
// #block(width: 100%, inset: 1em, stroke: (dash: "dashed", thickness: .1pt))[
// *Definition*
// ]
// Here I'll defin $TT$, as it will be used throught the whole document, and it would be nice to distinguish the time input from the rest of inputs easily 
// Sometimes $TT = RR$ (hard to tell, how do we define $t + 1$ in that case?), other times $TT == NN$
// Would it be a good idea to have $TT == QQ$? Nah, maybe, maybe not... I don't actually need all the values, I could define $TT$ based on a $Delta$ of time, I could take an interval $Delta$ and use it throught the case... something like $TT(Delta) = { q | q = n dot Delta forall n in N }$

// stroke: (thickness: .3pt, paint: black, dash: "dashed")
// #show raw.where(block: true): it => block(fill: luma(250), inset: 2em, width: 100%, stroke: (left: 2pt + luma(230), rest: (thickness: .3pt, paint: luma(230), dash: "dashed")), it)

// There are many kinds of system we would like model:
// - if we have to choose between a *monolithic* architecture or a *microservices* architecture, we would like to model the the servers, and, given a certain set of http requests over time, we would like to measure the costs
// - if we want to develop a medical device, we would like to model how the software on the device behaves based on patient's measurements
// - etc...
// #lorem(40) (introduction to models, and how we make them with DTMC) 

// === DTMC without inputs
// === DTMC


// $ 
//   p : X times X times U &-> [0, 1] \
//   (x, x', u) &|-> p(x'|x, u) \
// $
// $ 
//   g : X -> Y
// $

// if $U = emptyset$ then $p : X times X -> X$, otherwise, we couldn't have a transition function

// The cool thing is that DTMC's transition is influenced by the input... a different input value influences the transition probability. For example on input 1 we could have... on input 2 we could have... (maybe a rain example... if the input is umbrella, then we don't get wet, otherwise we get wet if it rains...) it could be a good example.
// (TODO: drawing) 

// for continuous spaces
// $
//   forall x space.en forall u space.en (x in X and u in U) ==> integral p(x'|x, u) d\x' = 1
// $


// we define as $U_t : M times NN -> U)$ as the input of $M$ at time $t$
// we define as $X_t : M times NN -> U)$ as the state of $M$ at time $t$
// we define as $Y_t : M times NN -> U)$ as the output of $M$ at time $t$
//
// What if I don't have an input? 
// How do I indicate the current state? The current output and the current input?
// How do I formally connect two DTMCs?


// Current state of the DTMC?

  // - 0: start
  // - 1: requirements engineering
  // - 2: development
  // - 3: testing

// #show raw.where(block: true): block.with(fill: luma(254), inset: 1em, width: 100%, stroke: (thickness: .1pt, paint: luma(100), dash: "dashed"))
// #show outline.entry.where(level: 1): set text(weight: "bold")
// #show outline.entry.where(level: 1): set repeat() 
// #show outline.entry.where(level: 1): it => {
//   show repeat : none
//   v(1.1em, weak: true)
//   text(size: 1.1em, weight: "bold", it)
// }
    // fill: black, inset: 0.1em,


// #set raw.text(size: 0.8em)
// #set list(indent: 1em, tight: false)
// #show list.where(): it => { block(inset: (left: .8em), it) }
// #show raw.where(block: true): it => block(fill: luma(250), inset: 2em, width: 100%, stroke: (thickness: .1pt, paint: luma(175), dash: "dashed"), it)
// fill: black, 
// align(center + horizon, text(white, size: 0.8em)[*#tag*])
// set text(size: .9em)


// \

// _"Software engineering is an engineering discipline that is concerned with all aspects of software production"_ @sommerville2016software. 

// \


// Systems // modeling 
// In the following pages I'll focus on the most *important concepts* of the course. Those fundamental concepts will be treated in *great detail* to give a deep enough understanding of the material.
// \

// as the uniform continuous distribution (TODO) $cal(U)(0, 1)$, maybe better if something around $cal(U)(0, 1) times cal(U)(0, 1)$, not like that, but around there
// and we define 
// And we connect the DTMC to itself with

// - $U = "Time" times RR$ 
// - $U(t + 1) = (t, r_1 + 1) "s.t." Y(t) = (r_0, r_1)$


// ```cpp
// #include <fstream>
// #include <random>
//
// typedef long double real_t;
//
// int main() {
//     std::random_device random_device;
//     std::default_random_engine random_engine(random_device());
//     std::uniform_real_distribution<real_t> random_state;
//
//     const size_t STATES_SIZE = 2, HORIZON = 10;
//     std::vector<real_t> states(STATES_SIZE, 0);
//
//     std::ofstream output("output.csv");
//
//     for (size_t time = 0; time <= HORIZON; time++) {
//         for (auto &state : states)
//             state = random_state(random_engine);
//
//         output << time << ' ';
//         for (auto state : states)
//             output << state << ' ';
//         output << std::endl;
//     }
//
//     output.close();
//
//     return 0;
// }
// ```
