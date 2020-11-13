# -*-coding:utf-8-*-
# author: zh-spike

from typing import Pattern
import requests
import json
import re
import getpass
from bs4 import BeautifulSoup
import csv

headers = {
        'User-Agent' : 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:46.0) Gecko/20100101 Firefox/46.0',
        'Content-Type': 'application/x-www-form-urlencoded',
        'Connection' : 'Keep-Alive',
        'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'
}

# 定义所需url
login_url = "https://www.ulearning.cn/umooc/user/login.do"  # 登录
info_url = "https://api.ulearning.cn/user"  # 获取用户信息的接口

# 启动一个session，用于登录交互
session = requests.Session()
# 存储单个章节信息
chapter = {"title": None, "id": None, "sections": []}
# token
token =""

# 登录函数
def login():
	auth = {}
	auth["name"] = input("[*] 用户名：")
	auth["passwd"] = getpass.getpass("[*] 密码(输入时不可见):")
	session.post(login_url, auth)
	if "token" in session.cookies.keys():
		global token
		token = session.cookies["token"]
		print("[+] 登录成功")
		return True
	else:
		print("[-] 登录失败")


# 显示用户信息
def print_user_info():
	res = session.get(info_url, headers={"UA-AUTHORIZATION": token})
	info = res.text.strip()
	info = json.loads(info)
	print("[*] 用户ID: " + str(info["userid"]))
	print("[*] 用户姓名: " + info["name"])

# 开始
def work():
    
	i = 0
	uri_header="https://www.ulearning.cn/umooc/user/study.do?operation=examReport&examid=33716&examuserid=7853135"
	with open('data.csv','w',encoding='utf-8') as f:
		while i < 127:
			res_header =session.get(uri_header, headers={"UA-AUTHORIZATION": token})
			res_header=res_header.text
			# soup = BeautifulSoup(res_header,'html.parser')
			# print(res_header)		
			examuser_id = re.findall('examuserid=(\d+)',res_header)
			paper_original = re.findall('paperID: "(\d+)"',res_header)
			exam_original = re.findall('examId: "(\d+)"',res_header)
			uri_header="https://www.ulearning.cn/umooc/user/study.do?operation=examReport&examid=33716&examuserid="+examuser_id[i]
			# print(uri_header)
			# print(examuser_id)
			# print(paper_original)
			# print(exam_original)
			uri_answer="https://www.ulearning.cn/umooc/learner/exam.do?operation=getCorrectAnswer&paperID="+paper_original[0]+"&examID="+exam_original[0]
			# print(uri_answer)
			uri_paper= "https://www.ulearning.cn/umooc/learner/exam.do?operation=getPaperForStudent&paperID="+paper_original[0]+"&examId="+exam_original[0] +"&userId=7753979"
			# print(uri_paper)
			res_paper = session.get(uri_paper, headers={"UA-AUTHORIZATION": token})
			res_answer = session.get(uri_answer, headers={"UA-AUTHORIZATION": token})
			res_paper=res_paper.text
			res_answer = res_answer.text
			# question_id = re.findall('"questionid":(\d+)',res_paper)
			children_id = re.findall('"questionid":(\d+),"score":\d,"title":"(.*?)"',res_paper)
			answer_id = re.findall('"(\d+)":{"correctAnswer":"(.*?)"',res_answer)
			number=0
			# print(children_id)	
			# print(answer_id)
			# print(len(question_id))
			while number < 75:
				qid = children_id[number][0]
				qtitle=children_id[number][-1]
				qtitle=	qtitle.replace("\n","")
				qtitle=	qtitle.replace(",","")
				correctid = answer_id[number][0]
				correctAnswer = answer_id[number][1]
				print('题号: {},题目: {},答案号: {},答案: {}'.format(qid,qtitle,correctid,correctAnswer))
				f.write("{},{},{},{}\n".format(qid,qtitle,correctid,correctAnswer))
				number += 1
			# print(res_paper)
			# print (res_answer)
			i += 1
		f.close()

def main():
	if not login():
		main()
	else:
		print_user_info()
		work()


if __name__ == '__main__':
	main()
