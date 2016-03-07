#!/usr/bin/env python
import math

ClasseA = "techlab"
fileOption = 'extend'

def readFromFile():
  arduFile = "/mnt/sda1/SD_A/collector/arduinoData"
  lines = []
  with open(arduFile, "r") as f:
      lines = f.readlines()
  with open(arduFile, "w") as f:
      f.truncate()
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

trace2 = Scatter(
  x=[],
  y=[],

)

#ARIA

aria_standard = [80] * len(time)

trace0 = Scatter(x=time,y=aria_array)
trace1 = Scatter(x=time,y=aria_standard)

dataAria = Data([trace0,trace1,trace2])
unique_url = py.plot(dataAria, filename = 'Respirabilita\'', fileopt=fileOption)

#SUONO

suono_standard = [210] * len(time)

trace0 = Scatter(x=time,y=suono)
trace1 = Scatter(x=time,y=suono_standard)

dataSuono = Data([trace0, trace1, trace2])
unique_url = py.plot(dataSuono, filename = 'Rumore', fileopt=fileOption)

#LUCE

luce_standard = [30] * len(time)

trace0 = Scatter(x=time,y=luce)
trace1 = Scatter(x=time,y=luce_standard)

dataLuce = Data([trace0, trace1, trace2])
unique_url = py.plot(dataLuce, filename = 'Luminosita\'', fileopt=fileOption)

#UMIDI

umidi_standard = [50] * len(time)

trace0 = Scatter(x=time,y=umidi)
trace1 = Scatter(x=time,y=umidi_standard)

dataUmidi = Data([trace0, trace1, trace2])
unique_url = py.plot(dataUmidi, filename = 'Umidita\'', fileopt=fileOption)

#TEMP

temp_standard = [19.5] * len(time)

trace0 = Scatter(x=time,y=temp)
trace1 = Scatter(x=time,y=temp_standard)

dataTemp = Data([trace0, trace1, trace2])
unique_url = py.plot(dataTemp, filename = 'Temperatura', fileopt=fileOption)


#CORRENTE

corrente_standard = [0] * len(time)

trace0 = Scatter(x=time,y=corrente_array)
trace1 = Scatter(x=time,y=corrente_standard)

dataCorrente = Data([trace0, trace1, trace2])
unique_url = py.plot(dataCorrente, filename = 'Consumo elettrico', fileopt=fileOption)
