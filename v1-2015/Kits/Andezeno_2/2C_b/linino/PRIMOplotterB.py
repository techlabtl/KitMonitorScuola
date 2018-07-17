#!/usr/bin/env python
import math

ClasseB = "2C"
fileOption = 'append'

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
corrente_array = map(lambda x: math.floor(x)*350, corrente)


import plotly.plotly as py
from plotly.graph_objs import *


#ARIA

trace2 = Scatter(
  x=time,
  y=aria_array,
  mode='lines',
  name=ClasseB,
    line=Line(
        width='2'
    )
)

dataAria = Data([trace2])
unique_url = py.plot(dataAria, filename = 'Respirabilita\'', fileopt=fileOption)

#SUONO

trace2 = Scatter(
  x=time,
  y=suono,
  mode='lines',
  name=ClasseB,
    line=Line(
        width='2'
    )
)

dataSuono = Data([trace2])
unique_url = py.plot(dataSuono, filename = 'Rumore', fileopt=fileOption)

#LUCE

trace2 = Scatter(
  x=time,
  y=luce,
  mode='lines',
  name=ClasseB,
    line=Line(
        width='2'
    )
)

dataLuce = Data([trace2])
unique_url = py.plot(dataLuce, filename = 'Luminosita\'', fileopt=fileOption)

#UMIDI

trace2 = Scatter(
  x=time,
  y=umidi,
  mode='lines',
  name=ClasseB,
    line=Line(
        width='2'
    )
)

dataUmidi = Data([trace2])
unique_url = py.plot(dataUmidi, filename = 'Umidita\'', fileopt=fileOption)


#TEMP

trace2 = Scatter(
  x=time,
  y=temp,
  mode='lines',
  name=ClasseB,
    line=Line(
        width='2'
    )
)

dataTemp = Data([trace2])
unique_url = py.plot(dataTemp, filename = 'Temperatura', fileopt=fileOption)


#CORRENTE

trace2 = Scatter(
  x=time,
  y=corrente_array,
  mode='lines',
  name=ClasseB,
    line=Line(
        width='2'
    )
)

dataCorrente = Data([trace2])
unique_url = py.plot(dataCorrente, filename = 'Consumo elettrico', fileopt=fileOption)
