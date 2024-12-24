# Cloth-Simulation
## Features
This is a Cpp cloth simulation inspired by https://www.youtube.com/watch?v=RjPBtxIupQA
Based on the app in the tutorial, I implemented the [Verlet integration](https://en.wikipedia.org/wiki/Verlet_integration), and new features like drag tearing, damping and extra bounding conditions.  
`Note that the app only depend on SFML.`

## The principle
Let the position of a particle at time $t$ be denoted as $\mathbf{r}(t)$, at time $t-1$ as $\mathbf{r}(t-1)$, and at time $t+1$ as $\mathbf{r}(t+1)$. The expressions for Verlet integration are as follows:

`Position update (predict future position):`
   $\mathbf{r}(t+1) = 2\mathbf{r}(t) - \mathbf{r}(t-1) + \mathbf{a}(t)\Delta t^2$ 
   where $\( \mathbf{a}(t) \)$ is the acceleration of the particle at time $\( t \)$, and $\( \Delta t \)$ is the time step.

`Position update (calculate past position):`
   $\mathbf{r}(t-1) = 2\mathbf{r}(t) - \mathbf{r}(t+1) - \mathbf{a}(t)\Delta t^2$ 
   This expression can be used to calculate the position of the particle at the previous time step if the current and future positions are known.

`Velocity calculation:`
   $\mathbf{v}(t) = \frac{\mathbf{r}(t+1) - \mathbf{r}(t-1)}{2\Delta t}$ 
   This expression can be used to calculate the velocity of the particle at time $t$ if the positions at the current and previous time steps are known.

In this context, $r$ represents the position vector, $a$ represents the acceleration vector, $v$ represents the velocity vector, and $\Delta
 t$ is the time step. The acceleration $a(t)$ is typically determined by the laws of physics (such as Newton's second law) or by external forces. 

 
Here is a demo animation for you to show the effect of my code.  

https://github.com/user-attachments/assets/73c2cc9e-46f7-46e0-a8f1-9997b1dca7ab
