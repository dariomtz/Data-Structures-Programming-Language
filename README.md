# Data Structures (DS) Programming Language 
This is a programming language that has simple data structures implemented from scratch. The main focus of this project is for new programmers to learn how some basic data structures work in the simplest way.

It was initialy called Shrek Programming language because there are some parts of the language that can be used with dialogs of the Shrek movies in spanish.

## How it works?
Once the executable program for main.c is built, run it and indicate the name of the file where your program is.

Every sentence in your program should be wrapped inside a function. There needs to be a function named "main", which will be the function to be excecuted. Other functions will only be executed if they are called.

### How to use it?

#### Functions
A function is a block of code that can be excecuted multiple times. The content of the function are bounded by curly braces, just as in the following example. A block of code is set of ordered sentences. The end of the sentence is marked by the line break character '\n'.

```
FUNCTION main(){
  print('Hello World!)
}
```
There two native functions to the language, that are help with standard input and output. They are called *input* and *print* respectively. 

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
-------------- | ----------------------- | --------------- | -------------------      | ------------------------------------
Stack          | ```STACK name```        | ```print()```   | ```name.print()```       | Prints the stack in between square brackets
.              | .                       | ```size()```    | ```name.size()```        | Returns an integer representing the size of the stack
.              | .                       | ```isEmpty()``` | ```name.isEmpty()```     | Returns a boolean that represents whether the stack is empty or not
.              | .                       | ```push(data)```| ```name.push(data)```    | Places the data received on top of the stack
.              | .                       | ```top()```     | ```name.top()```         | Returns the value that is on top of the stack
.              | .                       | ```pop()```     | ```name.pop()```         | Returns the value that is on top of the stack, as well as removing it from the stack
Queue          | ```QUEUE name```        | ```print()```   | ```name.print()```       | Prints the queue in between square brackets
.              | .                       | ```size()```    | ```name.size()```        | Returns an integer representing the size of the queue
.              | .                       | ```isEmpty()``` | ```name.isEmpty()```     | Returns a boolean that represents whether the queue is empty or not
.              | .                       | ```offer(data)```  | ```name.offer(data)```| Places the data received on the back of the queue
.              | .                       | ```peek()```    | ```name.peek()```        | Returns the value that is on the front of the queue
.              | .                       | ```poll()```    | ```name.poll()```        | Returns the value that is on the front of the queue, as well as removing it from the queue
List           | ```LIST name```         | ```print()```   | ```name.print()```       | Prints the list in between parenthesis
.              | .                       | ```size()```    | ```name.size()```        | Returns an integer representing the size of the list
.              | .                       | ```remove(int)```   | ```name.remove(int)```   | Removes the ith element of the list
.              | .                       | ```set(int,data)``` | ```name.set(int, data)```| Places the data received on in the ith position of the list. The position must be a valid position between 0 and size inclusive.
.              | .                       | ```add(data)``` | ```name.add(data)```     | Adds the data received after the last element of the list
.              | .                       | ```get(int)```  | ```name.pop(int)```      | Returns the value that is on the ith position of the list
Set            | ```SET name```          | ```print()```   | ```name.print()```       | Prints the set in between curly brackets
.              | .                       | ```size()```    | ```name.size()```        | Returns an integer representing the size of the set
.              | .                       | ```remove(data)```  | ```name.remove(data)```  | Deletes the data given, if it is inside the set already.
.              | .                       | ```add(data)``` | ```name.add(data)```     | Adds the data given to the set, if it isn't inside the set yet.
.              | .                       | ```contains(data)```| ```name.contains(data)```| Returns a boolean representing if the data given is already inside or not 
Map            | ```MAP name```          | ```print()```   | ```name.print()```       | Prints the map in between square brackets
.              | .                       | ```size()```    | ```name.size()```        | Returns an integer representing the size of the map
.              | .                       | ```put(key, value)```| ```name.put(key, value)```   | Saves the given value under the given key
.              | .                       | ```get(key)```  | ```name.get(key)```      | Gets the value saved under the given key
.              | .                       | ```remove(key)```     | ```name.top(key)```         | Deletes the given key and the value saved under it


#### Operations
The DS Programming Language supports all the following operations. It also supports parentesis ```()``` to specify priority.

##### Assignation

Operator  | Operation
--------- | ---------
=         | assign
+=        | sum and assign
-=        | subtract and assign
\*=       | multiply and assign
/=        | divide and assign
%=        | modulo and assign
++        | add 1 and assign
--        | subtract 1 and assign

##### Arithmetic

Operator  | Operation
--------- | ---------
+         | sum
-         | subtract
\*        | multiply
/         | divide
%         | modulo

##### Logical

Operator  | Operation
--------- | ---------
>         | bigger than
<         | smaller than
>=        | bigger or equal than
<=        | smaller or equal than
==        | equals
!=        | not equals
&&	      | logical and
||	      | logical or

##### Bitwise

Operator  | Operation
--------- | ---------
&	        | bitwise and
^	        | bitwise xor
|	        | bitwise or

#### Control flow statements

There is a ternary operator, to help you make decisions on the fly. For example:
```
  print((age >= 18) ? 'You are an adult' : 'You are underage')
```

##### Conditionals

###### IF
Condicionals have the following structure.

```
  if(condition){
      
  }
```

The condition can either be a boolean or a number, where zero represents false. The sentences inside the curly braces will be executed if and only if the condition evaluates to TRUE.

###### ELSE
Else statements are used to specify what must be done in case that the condition evaluates to FALSE.
```
  if(condition){
      
  }else{
  
  }
```

##### LOOPS

###### FOR
For loops have the following structure.
```
  for(initialization; condition; addition){
  
  }
``` 

Initialization is a sentence where an index can be initialized. Condition is the condition that must be met for the loop to continue, and addition is where the index can be modified before the next iteration.

An example of a for loop that prints the even numbers between 0 and 9 would be as the following.
```
  for(i = 0; i < 10; i+=2){
    print(i)
  }
```

##### WHILE

A while loop has the following structure.

```
  while(condition){
  
  }
```
This loops continues to execute the code in between curly braces as long as the condition evaluates to TRUE







