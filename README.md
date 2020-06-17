# Data Structures (DS) Programming Language 
This is a programming language that has simple data structures implemented from scratch. The main focus of this project is for new programmers to learn how some basic data structures work in the simplest way.

It was initialy called Shrek Programming language because there are some parts of the language that can be used with dialogs of the Shrek movies in spanish.

## How it works?
Once the executable program for main.c is built, run it and indicate the name of the file where your program is.

Every sentence in your program should be wrapped inside a function. There needs to be a function named "main", which will be the function to be excecuted. Other functions will only be executed if they are called.

### How to use it?

#### Functions

#### Variables
Variables are declared and initialized at the same time, just like in python:

```python
num = 3
```
##### Data types
In DS, there are basic data types such as:

* integer
* float
* string

For each of this type, there is a corresponding casting function. These return the value passed as an argument as the corresponding data type.

* ```int(data)```
* ```float(data)```
* ```str(data)```

You can also use variables to save the reference to a function. For example, if you have the function ```foo``` defined somewhere in your program, you can do this:

```foo_reference = foo```

This will allow you to use the function ```foo``` under the name ```foo_reference```. That means that functions can be passed as arguments to another function.

##### Data structures
There is another type of data that can be stored. However, these are rather structures than just raw data. These data structures allow you to organize information in different ways, explained in the following table:

Data Structure | Example of declaration  | Methods         | Example                  | Explanation
-------------- | ----------------------- | --------------- | -------------------      | -----------
Stack          | ```STACK name```        | ```print()```   | ```name.print()```       | Prints the stack in between brackets
               |                         | ```size()```    | ```name.size()```        | Returns an integer representing the size of the stack
               |                         | ```isEmpty()``` | ```name.isEmpty()```     | Returns a boolean that represents whether the stack is empty or not
               |                         | ```push(data)```| ```name.push(data)```    | Places the data received on top of the stack
               |                         | ```top()```     | ```name.top()```         | Returns the value that is on top of the stack
               |                         | ```pop()```     | ```name.pop()```         | Returns the value that is on top of the stack, as well as removing it from the stack



#### Operations

##### Assignation

##### Arithmetic

##### Logical

##### Bitwise

#### Control flow statements

##### IF

##### ELSE

##### FOR

##### WHILE








