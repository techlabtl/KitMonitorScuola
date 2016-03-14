#!/usr/bin/env python
import math

dataName = "3C"
fileOption = 'new'

def readFromFile():
  arduFile = "/mnt/sda1/arduino/datalog.txt"
  lines = []
  with open(arduFile, "r") as f:
      lines = f.readlines()
  lines = map(lambda x: x.rstrip(), lines)
  return lines

lines = readFromFile()

i=0

data = []
ora = []
aria = []
suono = []
luce = []
umidi = []
temp = []
corrente = []

for line in lines:
    data.append(lines[i].split(',')[0])
    ora.append(lines[i].split(',')[1])
    aria.append(lines[i].split(',')[2])
    luce.append(lines[i].split(',')[3])
    suono.append(lines[i].split(',')[4])
    umidi.append(lines[i].split(',')[5])
    temp.append(lines[i].split(',')[6])
    corrente.append(lines[i].split(',')[7])
    i=i+1


#TIME
time = []

for f, b in zip(data, ora):
    time.append("20"+f.split('/')[2]+"-"+f.split('/')[0]+"-"+f.split('/')[1]+" "+b)

#ARIA

Xa = 10
Ya = 100
Xb = 150
Yb = 0
aria = map(int,aria)
aria_array = map(lambda x: ((Yb-Ya)/float(Xb-Xa))*(x-Xa)+Ya, aria)

#SUONO
#suono = map(int,suono)
#suono_array = np.asarray(suono)
#suono_array = map(lambda x: (math.pow(float(x),1.75)/2400)+31, suono_array)

#LUM AS IS

#CORRENTE
corrente = map(float,corrente)
corrente_array = map(lambda x: math.floor(x)*300, corrente)


import plotly.plotly as py
from plotly.graph_objs import *


#ARIA
aria_standard = [80] * len(time)

trace0 = Scatter(
  x=time,
  y=aria_array,
  mode='lines',
  name=dataName,
    line=Line(
        width='2'
    )
)

trace1 = Scatter(
  x=time,
  y=aria_standard,
  mode='lines',
  name="buona aria",
    line=Line(
        width='1'
    )
)

dataAria = Data([trace0, trace1])
layout = Layout(
    yaxis=YAxis(
        title="indice di qualita\' dell'aria",
    ),
    autosize=True,
    title="Respirabilita\'"
)
fig = Figure(data=dataAria, layout=layout)
unique_url = py.plot(fig, filename = 'Respirabilita\'', fileopt=fileOption)


#SUONO

suono_standard = [210] * len(time)

trace0 = Scatter(
  x=time,
  y=suono,
  mode='lines',
  name=dataName,
    line=Line(
        width='2'
    )
)
trace1 = Scatter(
  x=time,
  y=suono_standard,
  mode='lines',
  name="silenzio",
    line=Line(
        width='1'
    )
)

dataSuono = Data([trace0, trace1])
layout = Layout(
    yaxis=YAxis(
        title="livello di pressione sonora",
    ),
    autosize=True,
    title="Rumore"
)
fig = Figure(data=dataSuono, layout=layout)
unique_url = py.plot(fig, filename = 'Rumore', fileopt=fileOption)

#LUCE

luce_standard = [30] * len(time)

trace0 = Scatter(
  x=time,
  y=luce,
  mode='lines',
  name=dataName,
    line=Line(
        width='2'
    )
)
trace1 = Scatter(
  x=time,
  y=luce_standard,
  mode='lines',
  name="buona luminosita\'",
    line=Line(
        width='1'
    )
)


dataLuce = Data([trace0, trace1])
layout = Layout(
    yaxis=YAxis(
        title="livello di luminosita\'",
    ),
    autosize=True,
    title="Luminosita\'"
)
fig = Figure(data=dataLuce, layout=layout)
unique_url = py.plot(fig, filename = 'Luminosita\'', fileopt=fileOption)



#UMIDI

umidi_standard = [50] * len(time)

trace0 = Scatter(
  x=time,
  y=umidi,
  mode='lines',
  name=dataName,
    line=Line(
        width='2'
    )
)
trace1 = Scatter(
  x=time,
  y=umidi_standard,
  mode='lines',
  name="buona umidita\'",
    line=Line(
        width='1'
    )
)

dataUmidi = Data([trace0, trace1])
layout = Layout(
    yaxis=YAxis(
        title="Percentuale di umidita\' relativa",
    ),
    autosize=True,
    title="Umidita\'"
)
fig = Figure(data=dataUmidi, layout=layout)
unique_url = py.plot(fig, filename = 'Umidita\'', fileopt=fileOption)



#TEMP

temp_standard = [19.5] * len(time)

trace0 = Scatter(
  x=time,
  y=temp,
  mode='lines',
  name=dataName,
    line=Line(
        width='2'
    )
)
trace1 = Scatter(
  x=time,
  y=temp_standard,
  mode='lines',
  name="buona temperatura",
    line=Line(
        width='1'
    )
)

dataTemp = Data([trace0, trace1])
layout = Layout(
    yaxis=YAxis(
        title="Temperatura in gradi Celsius",
    ),
    autosize=True,
    title="Temperatura"
)
fig = Figure(data=dataTemp, layout=layout)
unique_url = py.plot(fig, filename = 'Temperatura', fileopt=fileOption)



#CORRENTE

corrente_standard = [0] * len(time)

trace0 = Scatter(
  x=time,
  y=corrente_array,
  mode='lines',
  name=dataName,
    line=Line(
        width='2'
    )
)
trace1 = Scatter(
  x=time,
  y=corrente_standard,
  mode='lines',
  name="bassi consumi",
    line=Line(
        width='1'
    )
)

dataCorrente = Data([trace0, trace1])
layout = Layout(
    yaxis=YAxis(
        title="Assorbimento di corrente",
    ),
    autosize=True,
    title="Consumo elettrico in Watt"
)
fig = Figure(data=dataCorrente, layout=layout)
unique_url = py.plot(fig, filename = 'Consumo elettrico', fileopt=fileOption)
