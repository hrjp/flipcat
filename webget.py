from datetime import date
import datetime
import numpy as np
import matplotlib.pyplot as plt
import openpyxl
import os
import pandas as pd
import unicodedata
import re
import csv
import tabula

#output_file_path=os.path.dirname(__file__)+"/csv/"
output_file_path="excel_converter/csv/"
def webpdf_to_csv():
    dt_now = datetime.datetime.now()
    month=dt_now.month-1
    pdffile_path="http://www.kobe-kosen.ac.jp/groups/coop/coopyotei"+str(month)+".pdf"

    condata = tabula.read_pdf(pdffile_path, stream=True,pages="1")
    df=pd.DataFrame(condata[0])
    #print(df.iloc[2,3])

    #営業時間部分を抽出
    df2=df.iloc[0:,[2,3]]


    for j in range(2):
        for i in range(len(df2)):
            if df2.iloc[i,j]=="休業":
                df2.iloc[i,j]="-1:-1~-1:-1"
            #else:
                #全角文字を半角に変換
                #text=df2.iloc[i,j]
                #df2.iloc[i,j]=unicodedata.normalize("NFKC", text)

    out_list=[]

    for k in range(len(df2)):
        out_list.append(re.split('[:~]',df2.iloc[k,0])+re.split('[:~]',df2.iloc[k,1]))
    df3=pd.DataFrame(out_list)
    return df3

dt_now = datetime.datetime.now()
year=dt_now.year
month=dt_now.month-1
#シート選択
if month<10:
    decade="0"
else:
    decade=""

file_path=output_file_path+str(year)+decade+str(month)+".csv"
print(os.path.dirname(__file__))
#csvに変換して出力
webpdf_to_csv().to_csv(file_path, index = False,header=False)

