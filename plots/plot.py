import plotly.plotly as py
from plotly.graph_objs import *

'''cores = [1, 2, 4, 8, 16, 32]

trace1 = Bar(
    x=['Cores : ' + str(i) for i in cores],
    y=[1, 2, 3, 4, 5, 6],
    name='scattered'
)
trace2 = Bar(
    x=['Cores : ' + str(i) for i in cores],
    y=[6, 5 ,4, 3, 2, 1],
    name='non scattered'
)
data = Data([trace1, trace2])
layout = Layout(
    barmode='group'
)
fig = Figure(data=data, layout=layout)
py.plot (fig)
#plot_url = py.plot(fig, filename='grouped-bar')'''

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


results1 = [1, 2, 3, 4, 5, 6]
results2 = [6, 5, 4, 3, 2, 1]
plotResults (results1, results2, '1 Lab')