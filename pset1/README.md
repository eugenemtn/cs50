# [Problem set 1: C](https://docs.cs50.net/2019/x/psets/1/index.html)


#### Solutions
+ `mario.c`, a program that prints out a pyramid of a specified height, per the below.
```
$ ./mario
Height: 5
    # #
   ## ##
  ### ###
 #### ####
##### #####
```
+ `cash.c` a program that calculates the minimum number of coins required to give a user change.
```
$ ./cash
Change owed: 
0.41
4
```
+ `credit.c` this piece of code determines whether a provided credit card number is valid or not. 
Can detect MasterCard, Amex and Visa cards.
```
$ ./credit
Number: 5168755629319142 
MASTERCARD
$ ./credit
Number: 5168755629319145
INVALID
```