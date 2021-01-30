import csv
import numpy as np
import ParEstAlgorithm
import matplotlib.pyplot as plt

with open ('D:/Study/硕士/我的工作/硕士学位课程/模式识别/实验1/SampleLabels.csv') as csvfile:
    reader = csv.reader(csvfile)
    rows = [row for row in reader]
    for i in range(len(rows)):
        for j in range(len(rows[i])):
            rows[i] = int(rows[i][j])
DyOrigin = []
for i in range(len(rows)):
    DyOrigin.append(rows[i])
DyOrigin = np.array(DyOrigin)


with open('D:/Study/硕士/我的工作/硕士学位课程/模式识别/实验1/ClusterSamples.csv') as csvfile:
    reader = csv.reader(csvfile)
    rows = [row for row in reader]
    print(len(rows))
    print(len(rows[0]))
    for i in range(len(rows)):
        for j in range(len(rows[i])):
            rows[i][j] = int(rows[i][j])

PureData = []
for i in range(len(rows)):
    PureData.append(rows[i])
PureData = np.array(PureData)

print("DyOrigin:",DyOrigin)
print("PureData:",PureData)

plt.figure(1)
for seedi in range(5):
    print("seedi",seedi)
    np.random.seed(seedi) #使得每次都随机都一样
    dim = len(rows[0])
    numM = 10
    DataContainDy = ParEstAlgorithm.K_means(PureData,numM)

    ContVarietyOrigin = []
    for i in range(numM):
        ContVarietyOrigin.append(0)

    ContRightClassify = []
    for i in range(numM):
        ContRightClassify.append(0)

    CorrectClassifyRate = []
    for i in range(numM):
        CorrectClassifyRate.append(0)

    for i in range(len(DyOrigin)):
        ContVarietyOrigin[DyOrigin[i]] = ContVarietyOrigin[DyOrigin[i]] + 1
        if DyOrigin[i] == DataContainDy[i][1]:
            ContRightClassify[DyOrigin[i]] = ContRightClassify[DyOrigin[i]] + 1

    for i in range(numM):
        CorrectClassifyRate[i] = round(100*(ContRightClassify[i]/ContVarietyOrigin[i]),3)

    print("ContVarietyOrigin",ContVarietyOrigin) #各类别原始数据个数
    print("ContRightClassify",ContRightClassify) #分类正确的各类别数据个数
    print("CorrectClassifyRate",CorrectClassifyRate)

    plt.plot(np.linspace(0,numM-1,numM),CorrectClassifyRate)
    plt.annotate(str(seedi), xy=(0, CorrectClassifyRate[0]), xytext=(0, CorrectClassifyRate[0] - 1), arrowprops=dict(arrowstyle='->'))

    # for  i in range(len(DataContainDy)):
plt.show()

