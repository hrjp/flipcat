import numpy as np
import matplotlib.pyplot as plt
import openpyxl
import os
import pandas as pd
import unicodedata
import re

#エクセルファイルロード
wb = openpyxl.load_workbook(os.path.dirname(__file__)+"/xlsx/cafeteria_schedule.xlsx")
#シート選択
ws = wb['2021.10']

# dataframeに変換
df = pd.DataFrame(ws.values)
#営業時間部分を抽出
df2=df.iloc[4:,[3,9]]



for j in range(2):
    i=0
    while not(df2.iloc[i,j] is None):
    #for k in range(30):
        if df2.iloc[i,j]=="休業":
            df2.iloc[i,j]=-1
        else:
            #全角文字を半角に変換
            text=df2.iloc[i,j]
            df2.iloc[i,j]=unicodedata.normalize("NFKC", text)
        i+=1
df2=df2.head(i)
print(df2)
print(re.split('[:~]',df2.iloc[0,0]))