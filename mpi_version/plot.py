import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()
ax = fig.add_subplot(111)

## the data
N = 5
firstVar = [18, 35, 30, 35, 27, 42]
menStd =   [2, 3, 4, 1, 2]
secondVar = [25, 32, 34, 20, 25, 38]
womenStd =   [3, 5, 2, 3, 3]

## necessary variables
ind = np.arange(N)                # the x locations for the groups
width = 0.35                      # the width of the bars

## the bars
rects1 = ax.bar(ind, firstVar, width,
                color='black')

rects2 = ax.bar(ind+width, secondVar, width,
                    color='red')

# axes and labels
ax.set_xlim(-width,len(ind)+width)
ax.set_ylim(0,45)
ax.set_ylabel('Time')
ax.set_title('Time for 1st and 2nd version of task')
cores = array ('i', 1, 2, 4, 8, 16, 32)
xTickMarks = ['Cores : '+str(i) for i in cores]
ax.set_xticks(ind+width)
xtickNames = ax.set_xticklabels(xTickMarks)
plt.setp(xtickNames, rotation=45, fontsize=10)

## add a legend
ax.legend( (rects1[0], rects2[0]), ('non scattered', 'scattered') )

plt.show()

