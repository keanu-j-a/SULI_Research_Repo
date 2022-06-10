#!/usr/bin/env python
# coding: utf-8

# In[8]:


# Keanu J. Ammons
# Modified on: 6/9/2022


# Keanu J. Ammons preliminary study
# This code is a prelimary analysis of a parallel computing problem.

# Problem statement is as follows:

# Find the sum of all the even-valued terms in the Fibonacci sequence
# which do not exceed four million.

# This inital case problem is easily scaled up or down to increase or decrease the task length and/or computation time of an
# algorithim. For these reasons, this case problem makes a good inital situation to study.

# The correct answer is 4613732 


# In[9]:


# External imports:
# Note that "pip install pyRAPL" and "pip install pymongo" is required for this preliminary study
# "pip install py-cpuinfo==5.0.0" may be necessary when "pip intsall cpuinfo" does not work
import numpy as np
import cpuinfo
import psutil
import platform

# this import allows functions and commands to be executed simultaneously
# this will be useful to ensure timing sequences are started independed of the FibbSolver function
# from multiprocessing import Process


# In[10]:


# SINGLE CORE COMPUTATION
time_list = []
cpu_percentage = []
j = 0

# Define the inital parameters for the python function


# In[13]:


# A basic function that performs the test for a specified number of times.

def FibbSolver(TestCount):
    # This while loop is implmented so that the fibb probelm is solved for a certain number of times
    # This was added so that a significant cpu % percantge will be generated (to generate data for the single node case)
    i = 0
    while i < TestCount:
        
        # for some reason, import time would not work unless it was in the same cell as the 
        import time
        # SINGLE CORE COMPUTATION
        # Set a timer in milseconds to record the time 
        start_time_miliseconds = round(time.time()* 1000)
        fibo_even  = [2]
        num1 = 1
        num2 = 2
        sumNum = num1 + num2
        
        # document cpu usage information.
        # The entire loop will not be complete until a continious data stream of all of the cpu data is gathered
        x = psutil.cpu_percent(interval=.25, percpu=False)
        cpu_percentage.append(x)
        
        while sumNum < 4000001:
            
            # Sum the two numbers in the sequence
            sumNum = num1 + num2
            
            # Directly update the two numbers
            num1 = num2
            num2 = sumNum
            
            # Check and see if your current number is odd or even. if even, append to a list
            if sumNum % 2 == 0:
                fibo_even.append(sumNum)
                
        # print and sum the final list for the sake of data analysis
        #print("   --- %s is the sum ---" % sum(fibo_even))
        #print("")
        #print("Computation time is listed below: ")
        #print("")
        
        # absolute value is used to ensure all delta time values are positive
        time = abs(time.time() * 1000 - start_time_miliseconds)
        
        #optiol print statement
        #print(" --- %s miliseconds ---" % time)
        
        time_list.append(time)
        i = i + 1


# In[14]:


# Conduct a basic test and compute the power behavior of the system
TestCount = 100
i = 0
time_list = []
cpu_percentage = []

FibbSolver(TestCount)

# print to the terminal (so the programmer knows what is going on ;)   )
print("")
print("These are the computation times for each item in ms:")
print(time_list)
arr1 = np.array(time_list)

# Perform some preliminary data analysis. Reminder that the time is in miliseconds
TotalCompTime = arr1.sum()
AverageCompTime = arr1.mean()
print(len(arr1))

print("")
print("This is the compliation time in ms:")
print(TotalCompTime)

# provide some basic analytical data for the process
# print("These are the cpu percentages for the entire operation at each interval.")
# true_cpu_percentage = list(filter(lambda element: element != 0,cpu_percentage))
# print(true_cpu_percentage)
# print(len(true_cpu_percentage))

print("")
print("This is the cpu usage:")
print(cpu_percentage)


# In[7]:


# true_cpu_percentage = list(filter(lambda element: element != 0,cpu_percentage))
# print(true_cpu_percentage)

# Originally, the array was 3121 elements in length with mostly zeros. Therefore, I reasoned that multiple cpu_measurements 
# were being taken in between each sucessive computation. Since the cpu must be in operation for all moments
# in the computation, I wrote a filter expression that dropped all zero elemnets of the array and printed result.

# To further justify this decision, the following is a direct excerpt from the psuitil documentation:
# the first time this function is called with 
# interval = 0.0 or None it will return a meaningless 0.0 value which you are supposed to ignore.


# In[15]:


print(len(cpu_percentage))


# In[16]:


print(len(time_list))


# In[17]:


import seaborn as sns
import matplotlib.pyplot as plt
get_ipython().run_line_magic('matplotlib', 'inline')


# In[ ]:


sns.violinplot(x="day", y="total_bill", data=tips,palette='rainbow')

