import plotly.plotly as py
from plotly.graph_objs import *

def plotResults (first_results = [], second_results = [], filename = 'Test'):
	cores = [1, 2, 4, 8, 16, 32]
	cores = ['Cores : ' + str(i) for i in cores]

	trace1 = Bar(
		x = cores,
		y = first_results,
		name = 'First'
	)

	trace2 = Bar(
		x = cores,
		y = second_results,
		name = 'Second'
	)
	data = Data([trace1, trace2])
	layout = Layout (
		barmode = 'group'
	)
	fig = Figure(data=data, layout=layout)
	plot_url = py.plot(fig, filename=filename)
	return


def calcSpeed (results = []): 
	speed = []

	for indx, result in enumerate(results):
		total = sum(results[:(indx + 1)]) 
		speed.append (result / total) 
	return speed

def calcEfficiency (speed = []):
	efficiency = []
	for indx, var in enumerate(speed):
		proc_num = 2**indx
		efficiency.append (var / proc_num)
	return efficiency

results1 = [5.92, 24.48, 13.26, 4.0, 5.0, 6.0]
results2 = [6.0, 5.0, 4.0, 3.0, 2.0, 1.0]
plotResults (results1, results2, '1 Lab')
speed1 = calcSpeed (results1)
speed2 = calcSpeed (results2)
plotResults (speed1, speed2, 'Acceleration')
efficiency1 = calcEfficiency (speed1)
efficiency2 = calcEfficiency (speed2)
plotResults (efficiency1, efficiency2, 'Efficiency')