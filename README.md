<p align="center">
  <img src="Philosophers_42_transparent_cropped.png" alt="philos"/>
</p>

## Table of Contents
1. [Introduction](#introduction)
2. [System Requirements](#system-requirements)
3. [Installation](#installation)
4. [Input Parameters](#input-parameters)
5. [Key Functions](#key-functions)

## Introduction

This project manages a multithreaded program using mutex locking mechanisms. It is inspired by the classic mathematical conundrum known as "The Dining Philosophers Problem." In this scenario, multiple philosophers are seated around a circular table with a single fork placed between each pair. Each philosopher alternates between thinking, eating, and sleeping; however, a philosopher requires two forks to eat. after a certain time (define by the user) which he did not eaten , he will die.

In this project, each fork is represented by a mutex that controls access to eating - which is the limited resource. To allow each philosopher to eat without dying of starvation, the system must be synchronized in such a way that it avoids "deadlock." Deadlock could occur if each philosopher picks up the left fork and then waits indefinitely for the right fork to be released by the neighbor. This scenario would result in the philosophers never getting to eat, metaphorically dying from the agony of unfulfilled pasta cravings.


<p align="center">
  <img src="philos_eating.webp" alt="My Image" width="400"/>
</p>

## System Requirements

The program has been tested on the following operating systems:

* Linux
* macOS

Both systems support the multithreaded operations required by this project, although the thread management in Linux and macOS differs slightly, which might impact the performance and behavior of the program.

## Installation

to install, clone the project first:
```bash
git clone git@github.com:Talkashi1111/Philosophers_42.git
```
than enter to the repository cloned and type :
```bash
# Compile the program (make sure you have a C compiler installed)
make
```
this make file has also a make data race rule : which uses drd ("Data Race Detector") tool designed by valgrind to detect data races in multithreaded programs.

## Input Parameters

To run the program, use:
```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```
The program accepts the following user inputs:

1. Number of Philosophers: Specifies the number of threads.
2. Time to Die: Duration for which each philosopher will die if not eat.
3. Time to Eat: Duration for which each philosopher will eat.
4. Time to Sleep: Duration for which each philosopher will sleep.
5. Number of Meals (Optional): Number of times each philosopher is required to eat.

## Key Functions

1. ft_join_thread: Waits for all threads to complete. Essential for preventing threads from running in the background after the main process exits, which could lead to memory leaks or other undesirable behaviors.
2. safe_mutex_handle and safe_thread_handle: These functions ensure robust handling of mutex and thread operations (lock, unlock, init, destroy, join, detach, create). They provide safeguards against unexpected return values and errors.
