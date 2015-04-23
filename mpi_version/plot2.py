import plotly.plotly as py
from plotly.graph_objs import *

cores = [1, 2, 4, 8, 16, 32]

trace1 = Bar(
    x=['Cores : ' + str(i) for i in cores],
    y=[20, 14, 23, 52, 54, 8],
    name='scattered'
)
trace2 = Bar(
    x=['Cores : ' + str(i) for i in cores],
    y=[12, 18, 29, 4, 6, 8],
    name='non scattered'
)
data = Data([trace1, trace2])
layout = Layout(
    barmode='group'
)
fig = Figure(data=data, layout=layout)
py.plot (fig)
#plot_url = py.plot(fig, filename='grouped-bar')