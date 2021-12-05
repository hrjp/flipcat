from datetime import date
import numpy as np
import matplotlib.pyplot as plt
import openpyxl
import os
import pandas as pd
import unicodedata
import re
import csv

#ファイルのパス
input_file_path=os.path.dirname(__file__)+"/xlsx/cafeteria_schedule.xlsx"
output_file_path=os.path.dirname(__file__)+"/csv/"


# 実数に変換できるか判定
def is_num(s):
    try:
        float(s)
    except ValueError:
        return False
    else:
        return True

# シートをcsvに変換して出力
def excel_to_csv(seat):
    # dataframeに変換
    df = pd.DataFrame(seat.values)
    #営業時間部分を抽出
    df2=df.iloc[4:,[3,9]]

    for j in range(2):
        i=0
        while not(df2.iloc[i,j] is None):
        #for k in range(30):
            if df2.iloc[i,j]=="休業":
                df2.iloc[i,j]="-1:-1~-1:-1"
            else:
                #全角文字を半角に変換
                text=df2.iloc[i,j]
                df2.iloc[i,j]=unicodedata.normalize("NFKC", text)
            i+=1
    
    df2=df2.head(i)
    out_list=[]

    for k in range(len(df2)):
        out_list.append(re.split('[:~]',df2.iloc[k,0])+re.split('[:~]',df2.iloc[k,1]))
    df3=pd.DataFrame(out_list)
    return df3


#エクセルファイルロード
wb = openpyxl.load_workbook(input_file_path)

for sheet in wb.sheetnames:
    if is_num(sheet):
        year=int(re.split('[.]',sheet)[0])
        month=int(re.split('[.]',sheet)[1])
        #シート選択
        ws = wb[sheet]
        if month<10:
            decade="0"
        else:
            decade=""

        file_path=output_file_path+str(year)+decade+str(month)+".csv"
        #csvに変換して出力
        excel_to_csv(ws).to_csv(file_path, index = False,header=False)

print("xlsx --> csv converted")