#!/usr/bin/env python
# coding: utf-8

# In[4]:


# Keanu J. Ammons
# Modified on: 6/9/2022


# Keanu J. Ammons preliminary study
# This code is a prelimary analysis of a parallel computing problem.

# Problem statement is as follows:

# Find the sum of all the even-valued terms in the Fibonacci sequence
# which do not exceed four million.

# The correct answer is 4613732 


# In[30]:


import time

# SINGLE CORE COMPUTATION

# Set a timer in milseconds to record the time 
start_time_miliseconds = round(time.time() * 1000)

fibo_even  = [2]
num1 = 1
num2 = 2
sumNum = num1 + num2

while sumNum < 4000001:
    # Sum the two numbers in the sequence
    sumNum = num1 + num2
    
    # Directly update the two numbers
    num1 = num2
    num2 = sumNum
    
    # Check and see if your current number is odd or even. if even, append to a list
    if sumNum % 2 == 0:
        fibo_even.append(sumNum)

# print and sum the final list
print("   --- %s is the sum ---" % sum(fibo_even))

print("")

print("Computation time is listed below: ")

print("")

print(" --- %s miliseconds ---" % (time.time() * 1000 - start_time_miliseconds))


# In[ ]:





# In[ ]:





# In[ ]:




