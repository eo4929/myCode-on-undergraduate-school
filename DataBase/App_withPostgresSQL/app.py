from flask import Flask, render_template, redirect, request
import psycopg2 as pg
import psycopg2.extras
import csv
from collections import Counter
import json
import ast
from datetime import datetime

app = Flask(__name__)

session = {'semail' : None, 'passwd' : None}
semail = '' # 함수마다 새로 초기화 됨;;
passwd = '' # 함수마다 새로 초기화 됨;;
storenames = [] # seller가 소유한 가게들의 이름리스트 -> 함수마다 새로 초기화 됨;;

db_connect = { #내 계정에 대한 정보 적어야 함
        'host': '127.0.0.1',
        'port': '5432',
        'user': 'postgres',
        'dbname': 'postgres',
        'password': 'eo030201'
    }
connect_string = "host={host} user={user} dbname={dbname} password={password} port={port}".format(**db_connect)

@app.route("/")
def index():
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    try: #가게의 설명과 태그 정보를 위해 사용할 테이블
        sql = f"CREATE TABLE sellerAndStore (sid integer, description char(30), tag1 char(10), tag2 char(10), primary key(sid))"
        print(sql)
        cur.execute(sql)
        conn.commit()
    except:
        print("sellerAndStore table already exists")

    conn.close()
    return render_template("index.html")


@app.route("/login", methods=['POST'])
def login():
    print('login func!')
    
    semail = request.form.get('semail')
    session['semail'] = semail
    passwd = request.form.get('passwd')
    session['passwd'] = passwd

    conn = pg.connect(connect_string)
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    print(f"{semail}, {passwd}")

    email_String = semail
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0] 
    domain = email_Divided[1]

    seller_flag = False
    customer_flag = False
    delivery_flag = False

    try: # seller 먼저
        sql = f"SELECT * FROM seller S WHERE S.local='{local}' AND S.domain='{domain}' AND S.passwd='{passwd}'"
        cur.execute(sql)
        rows = cur.fetchall()
    
        if(len(rows) == 1):
            seller_flag = True

        sql = f"SELECT * FROM customer C WHERE C.local='{local}' AND C.domain='{domain}' AND C.passwd='{passwd}'"
        cur.execute(sql)
        rows = cur.fetchall()

        if(len(rows) == 1):
            customer_flag = True
        
        sql = f"SELECT * FROM delivery D WHERE D.local='{local}' AND D.domain='{domain}' AND D.passwd='{passwd}'"
        cur.execute(sql)
        rows = cur.fetchall()

        if(len(rows) == 1):
            delivery_flag = True
    except:
        print('sql in login func fail!')
    
    if( seller_flag == False and customer_flag == False and delivery_flag == False):
        return render_template('error.html', msg="Wrong ID OR Wrong PASSWORD")
    
    if( seller_flag == True and customer_flag == False and delivery_flag == False):
        sql = f"SELECT S2.sname FROM seller S1, store S2 WHERE S1.local='{local}' AND S1.domain='{domain}' AND S1.seller_id=S2.seller_id"
        cur.execute(sql)
        rows = cur.fetchall()

        storenames = []
        for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
            storenames.append(row[0])
            print(storenames)

        return render_template('seller.html', semail=semail , storenames=storenames)
    
    # 구매자 로그인 시 addressBook(si,gu,dong) payMethod_account(type, bid, acc_num) payMethod_card(type, card_num) html에 전달해줘야함
    if( seller_flag == False and customer_flag == True and delivery_flag == False):
        sql = f"SELECT C.phone FROM customer C WHERE C.local='{local}' AND C.domain='{domain}' AND C.passwd='{passwd}'"
        cur.execute(sql)
        rows = cur.fetchall()
        phone=''
        for row in rows:
            phone = row[0]
        phone=phone.rstrip()
        try:
            # 구매자로 로그인 시 주소록 테이블 생성
            sql = f"CREATE TABLE {local}{phone}andaddressBook (si char(10),gu char(10),dong char(10))"
            print(sql)
            cur.execute(sql)
            conn.commit()
        except:
            print(f"{local}{phone}andaddressBook table already exists")

        try:
            # 구매자로 로그인 시 결제수단(account) 테이블 생성 및 관련 데이터들 cumstomer 테이블에서 가져와 넣기
            sql = f"CREATE TABLE {local}{phone}andpayMethod_account (type char(8),bid char(3), acc_num char(20))"
            print(sql)
            cur.execute(sql)
            conn.commit()
            # 아래가 customer로부터 account관련 3가지 필드 가져오는 부분
            sql = f"select payments from customer where phone='{phone}'"
            cur.execute(sql)
            rows = cur.fetchall()
            pay = rows[0]['payments'] #그냥 pay는 JSON이라서 파이썬 에서 사용할 수 있도록 json.loads함수로 변형시켜야 함
            newpay = json.loads(pay)
            print(newpay)
            type=''
            bid=''
            acc_num=''
            for row in newpay:
                if(row['type'] == 'account'):
                    type = row['type']
                    bid = row['data']["bid"]
                    acc_num = row['data']["acc_num"]
                    #아래가 이 3가지 필드를 account 테이블에 넣는 부분
                    conn.commit()
                    sql = f"insert into {local}{phone}andpayMethod_account values('{type}','{bid}','{acc_num}')"
                    print(sql)
                    cur.execute(sql)
                    conn.commit()      
        except:
            print(f"{local}{phone}andpayMethod_account table already exists")
        
        try:
            # 구매자로 로그인 시 결제수단(card) 테이블 생성 및 관련 데이터들 cumstomer 테이블에서 가져와 넣기
            sql = f"CREATE TABLE {local}{phone}andpayMethod_card (type char(8), card_num char(20))"
            print(sql)
            cur.execute(sql)
            conn.commit()
            # 아래가 customer로부터 card관련 2가지 필드 가져오는 부분
            sql = f"select payments from customer where phone='{phone}'"
            cur.execute(sql)
            rows = cur.fetchall()
            pay = rows[0]['payments'] #그냥 pay는 JSON이라서 파이썬 에서 사용할 수 있도록 json.loads함수로 변형시켜야 함
            newpay = json.loads(pay)
            print(newpay)
            type=''
            card_num=''
            for row in newpay:
                if(row['type'] == 'card'):
                    type = row['type']
                    card_num = row['data']["card_num"]
                    #아래가 이 2가지 필드를 account 테이블에 넣는 부분
                    conn.commit()
                    sql = f"insert into {local}{phone}andpayMethod_card values('{type}','{card_num}')"
                    print(sql)
                    cur.execute(sql)
                    conn.commit()
        except:
            print(f"{local}{phone}andpayMethod_card table already exists")

        conn.commit()
        #주소록 테이블 내용 html에 전달
        sql = f"SELECT * FROM {local}{phone}andaddressBook"
        cur.execute(sql)
        rows = cur.fetchall()

        addressBook = []
        for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
            addressBook.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
        #결제수단(account) 테이블 내용 html에 전달
        sql = f"SELECT * FROM {local}{phone}andpaymethod_account"
        cur.execute(sql)
        rows = cur.fetchall()

        payMethod_account = []
        for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
            payMethod_account.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
        #결제수단(card) 테이블 내용 html에 전달
        sql = f"SELECT * FROM {local}{phone}andpaymethod_card"
        cur.execute(sql)
        rows = cur.fetchall()

        payMethod_card = []
        for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
            payMethod_card.append(row[0] + ' ' + row[1] )

        return render_template('customer.html', semail=semail, addressBook=addressBook, payMethod_account=payMethod_account, payMethod_card=payMethod_card)
    
    if( seller_flag == False and customer_flag == False and delivery_flag == True):
        return render_template('delivery.html', semail=semail)

    #이제 로그인 정보가 동일한 사용자가 있는 경우
    if( seller_flag == True and customer_flag == True and delivery_flag == False):
        return render_template('sameuser.html', msg="select seller or customer!")
    if( seller_flag == True and customer_flag == False and delivery_flag == True):
        return render_template('sameuser.html', msg="select seller or delivery!")
    if( seller_flag == False and customer_flag == True and delivery_flag == True):
        return render_template('sameuser.html', msg="select customer or delivery!")
    if( seller_flag == True and customer_flag == True and delivery_flag == True):
        return render_template('sameuser.html', msg="select sellor or customer or delivery!")

    conn.close()


@app.route("/relogin", methods=['POST'])
def relogin():
    print('relogin func!')
    print(session['semail'])

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0] 
    domain = email_Divided[1]

    select = request.form.get('select')

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sql = f"SELECT S2.sname FROM seller S1, store S2 WHERE S1.local='{local}' AND S1.domain='{domain}' AND S1.seller_id=S2.seller_id"
    cur.execute(sql)
    rows = cur.fetchall()

    storenames = []
    for row in rows:
        storenames.append(row[0])

    conn.close()

    if(select == 'seller'):
        return render_template('seller.html', semail=session['semail'], storenames=storenames)
    if(select == 'customer'):
        sql = f"SELECT C.phone FROM customer C WHERE C.local='{local}' AND C.domain='{domain}' AND C.passwd='{passwd}'"
        cur.execute(sql)
        rows = cur.fetchall()
        phone=''
        for row in rows:
            phone = row[0]
        phone=phone.rstrip()
        try:
            # 구매자로 로그인 시 주소록 테이블 생성
            sql = f"CREATE TABLE {local}{phone}andaddressBook (si char(10),gu char(10),dong char(10))"
            print(sql)
            cur.execute(sql)
            conn.commit()
        except:
            print(f"{local}{phone}andaddressBook table already exists")

        try:
            # 구매자로 로그인 시 결제수단(account) 테이블 생성 및 관련 데이터들 cumstomer 테이블에서 가져와 넣기
            sql = f"CREATE TABLE {local}{phone}andpayMethod_account (type char(8),bid char(3), acc_num char(20))"
            print(sql)
            cur.execute(sql)
            conn.commit()
            # 아래가 customer로부터 account관련 3가지 필드 가져오는 부분
            sql = f"select payments from customer where phone='{phone}'"
            cur.execute(sql)
            rows = cur.fetchall()
            pay = rows[0]['payments'] #그냥 pay는 JSON이라서 파이썬 에서 사용할 수 있도록 json.loads함수로 변형시켜야 함
            newpay = json.loads(pay)
            print(newpay)
            type=''
            bid=''
            acc_num=''
            for row in newpay:
                if(row['type'] == 'account'):
                    type = row['type']
                    bid = row['data']["bid"]
                    acc_num = row['data']["acc_num"]
                    #아래가 이 3가지 필드를 account 테이블에 넣는 부분
                    conn.commit()
                    sql = f"insert into {local}{phone}andpayMethod_account values('{type}','{bid}','{acc_num}')"
                    print(sql)
                    cur.execute(sql)
                    conn.commit()      
        except:
            print(f"{local}{phone}andpayMethod_account table already exists")
        
        try:
            # 구매자로 로그인 시 결제수단(card) 테이블 생성 및 관련 데이터들 cumstomer 테이블에서 가져와 넣기
            sql = f"CREATE TABLE {local}{phone}andpayMethod_card (type char(8), card_num char(20))"
            print(sql)
            cur.execute(sql)
            conn.commit()
            # 아래가 customer로부터 card관련 2가지 필드 가져오는 부분
            sql = f"select payments from customer where phone='{phone}'"
            cur.execute(sql)
            rows = cur.fetchall()
            pay = rows[0]['payments'] #그냥 pay는 JSON이라서 파이썬 에서 사용할 수 있도록 json.loads함수로 변형시켜야 함
            newpay = json.loads(pay)
            print(newpay)
            type=''
            card_num=''
            for row in newpay:
                if(row['type'] == 'card'):
                    type = row['type']
                    card_num = row['data']["card_num"]
                    #아래가 이 2가지 필드를 account 테이블에 넣는 부분
                    conn.commit()
                    sql = f"insert into {local}{phone}andpayMethod_card values('{type}','{card_num}')"
                    print(sql)
                    cur.execute(sql)
                    conn.commit()
        except:
            print(f"{local}{phone}andpayMethod_card table already exists")

        conn.commit()
        #주소록 테이블 내용 html에 전달
        sql = f"SELECT * FROM {local}{phone}andaddressBook"
        cur.execute(sql)
        rows = cur.fetchall()

        addressBook = []
        for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
            addressBook.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
        #결제수단(account) 테이블 내용 html에 전달
        sql = f"SELECT * FROM {local}{phone}andpaymethod_account"
        cur.execute(sql)
        rows = cur.fetchall()

        payMethod_account = []
        for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
            payMethod_account.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
        #결제수단(card) 테이블 내용 html에 전달
        sql = f"SELECT * FROM {local}{phone}andpaymethod_card"
        cur.execute(sql)
        rows = cur.fetchall()

        payMethod_card = []
        for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
            payMethod_card.append(row[0] + ' ' + row[1] )
        return render_template('customer.html', semail=session['semail'], addressBook=addressBook, payMethod_account=payMethod_account, payMethod_card=payMethod_card)
    if(select == 'delivery'):
        return render_template('delivery.html', semail=session['semail'])


@app.route("/change_sellerInfo", methods=['POST'])
def change_sellerInfo():
    print("change_sellerInfo func!")
    print(session['semail'])
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    domain = email_Divided[1]

    #seller가 소유한 가게의 이름 리스트를 storenames에 넣기
    sql = f"SELECT S2.sname FROM seller S1, store S2 WHERE S1.local='{local}' AND S1.domain='{domain}' AND S1.seller_id=S2.seller_id"
    cur.execute(sql)
    rows = cur.fetchall()
    
    storenames = []
    for row in rows:
        storenames.append(row[0])

    sname = request.form.get('sname')
    passwd = request.form.get('passwd')

    sql = f"UPDATE seller SET name='{sname}', passwd='{passwd}' WHERE local='{local}' AND domain='{domain}'"
    cur.execute(sql)
    conn.commit()

    conn.close()
    return render_template('seller.html', semail=session['semail'] , storenames=storenames)


@app.route("/display_store", methods=['POST'])
def display_store():
    print("display_store func!")
    print(session['semail'])
    
    sname = request.form.get('storename')
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]

    sellerAndStore = local + 'and' + sname # create table에 사용할 테이블명
    print(sellerAndStore)
    
    #먼저 sid 구해오기
    sql = f"select S.sid from store S where S.sname='{sname}'"
    cur.execute(sql)
    rows = cur.fetchall()
    sid=''
    print(sid)
    for row in rows:
        print(row[0])
        sid = row[0]

    try: # 태그 정보는 2개로 한정하자
        sql = f"insert into sellerAndStore values ({sid},'설명이 없습니다', '태그1', '태그2')"
        print(sql)
        cur.execute(sql)
        conn.commit()
    except:
        print('insert into sellerAndStore error! -> already inserted')

    conn.commit() #커밋을 하고 sql 다시 db에 보내니 문제 해결 ㅠㅠ
    sql = f"select description,tag1,tag2 from sellerAndStore where sid={sid}"
    print(sql)
    cur.execute(sql)
    rows = cur.fetchall()
    print(rows)
    storeDesc=''
    tag1=''
    tag2=''
    for row in rows:
        storeDesc=row[0]
        tag1=row[1]
        tag2=row[2]

    conn.close()
    return render_template('store.html', semail=session['semail'] , sname=sname , storeDesc=storeDesc, tag1=tag1,tag2=tag2)

@app.route("/insertOrchange_storeDesc", methods=['POST'])
def insertOrchange_storeDesc():
    print("insertOrchange_storeDesc func!")
    print(session['semail'])

    sname = request.form.get('sname')
    sdesc = request.form.get('sdesc')

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]

    sellerAndStore = local + 'and' + sname # create table에 사용할 테이블명
    print(sellerAndStore)

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    #먼저 sid 구해오기
    sql = f"select S.sid from store S where S.sname='{sname}'"
    cur.execute(sql)
    rows = cur.fetchall()
    sid=''
    print(sid)
    for row in rows:
        print(row[0])
        sid = row[0]

    sql = f"UPDATE sellerAndStore SET description='{sdesc}' where sid={sid}"
    print(sql)
    cur.execute(sql)
    conn.commit()

    sql = f"select description,tag1,tag2 from sellerAndStore where sid={sid}"
    print(sql)
    cur.execute(sql)
    rows = cur.fetchall()
    print(rows)
    storeDesc=''
    tag1=''
    tag2=''
    for row in rows:
        storeDesc=row[0]
        tag1=row[1]
        tag2=row[2]

    conn.close()
    return render_template('store.html', semail=session['semail'] , sname=sname , storeDesc=storeDesc, tag1=tag1,tag2=tag2)


@app.route("/insertOrchange_tag", methods=['POST'])
def insertOrchange_tag():
    print("insertOrchange_tag func!")
    print(session['semail'])

    sname = request.form.get('sname')
    tag1name = request.form.get('tag1name')
    tag2name = request.form.get('tag2name')

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]

    sellerAndStore = local + 'and' + sname # create table에 사용할 테이블명
    print(sellerAndStore)

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    #먼저 sid 구해오기
    sql = f"select S.sid from store S where S.sname='{sname}'"
    cur.execute(sql)
    rows = cur.fetchall()
    sid=''
    print(sid)
    for row in rows:
        print(row[0])
        sid = row[0]

    sql = f"UPDATE sellerAndStore SET tag1='{tag1name}',tag2='{tag2name}' where sid={sid}"
    print(sql)
    cur.execute(sql)
    conn.commit()

    sql = f"select description,tag1,tag2 from sellerAndStore where sid={sid}"
    print(sql)
    cur.execute(sql)
    rows = cur.fetchall()
    print(rows)
    storeDesc=''
    tag1=''
    tag2=''
    for row in rows:
        storeDesc=row[0]
        tag1=row[1]
        tag2=row[2]

    conn.close()
    return render_template('store.html', semail=session['semail'] , sname=sname , storeDesc=storeDesc, tag1=tag1,tag2=tag2)


@app.route("/store_menu", methods=['POST'])
def store_menu():
    print("store_menu func!")
    print(session['semail'])

    sname = request.form.get('sname')

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]

    sellerAndStoreAndMenu = local + 'and' + sname + 'andMenu' # create table에 사용할 테이블명 -> 사용안함
    print(sellerAndStoreAndMenu)

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sql = f"select M.menu from store S, menu M where S.sname='{sname}' AND S.sid=M.sid"
    print(sql)
    cur.execute(sql)
    rows = cur.fetchall()
    print(rows)
    menuList=[]
    for row in rows:
        print(row[0])
        menuList.append(row[0])
        
    conn.close()
    return render_template('store_menu.html', semail=session['semail'] , sname=sname , menuList=menuList)


@app.route("/insertMenu", methods=['POST'])
def insertMenu():
    print("insertMenu func!")
    print(session['semail'])

    sname = request.form.get('sname')
    menu = request.form.get('menu')

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    #먼저 sid 구해오기
    sql = f"select distinct M.sid from store S, menu M where S.sname='{sname}' AND S.sid=M.sid"
    cur.execute(sql)
    rows = cur.fetchall()
    sid=''
    print(sid)
    for row in rows:
        print(row[0])
        sid = row[0]

    # 그다음 새로운 메뉴 삽입하기
    sql = f"insert into menu values ('{menu}','{sid}')"
    print(sql)
    cur.execute(sql)
    conn.commit()

    # 추가된 메뉴 보함하는 메뉴 리스트 다시 html에 전달하여 화면에 뿌려주기
    sql = f"select M.menu from store S, menu M where S.sname='{sname}' AND S.sid=M.sid"
    print(sql)
    cur.execute(sql)
    rows = cur.fetchall()
    print(rows)
    menuList=[]
    for row in rows:
        print(row[0])
        menuList.append(row[0])

    conn.close()
    return render_template('store_menu.html', semail=session['semail'] , sname=sname , menuList=menuList)


@app.route("/changeMenu", methods=['POST'])
def changeMenu():
    print("changeMenu func!")
    print(session['semail'])

    sname = request.form.get('sname')
    menu = request.form.get('menu')
    newMenu = request.form.get('newMenu')

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    #먼저 sid 구해오기
    sql = f"select distinct M.sid from store S, menu M where S.sname='{sname}' AND S.sid=M.sid"
    cur.execute(sql)
    rows = cur.fetchall()
    sid=''
    print(sid)
    for row in rows:
        print(row[0])
        sid = row[0]

    # 그다음 메뉴 변경하기
    sql = f"update menu set menu='{newMenu}' where sid='{sid}' AND menu='{menu}'"
    print(sql)
    cur.execute(sql)
    conn.commit()

    # 변경된 메뉴 보함하는 메뉴 리스트 다시 html에 전달하여 화면에 뿌려주기
    sql = f"select M.menu from store S, menu M where S.sname='{sname}' AND S.sid=M.sid"
    print(sql)
    cur.execute(sql)
    rows = cur.fetchall()
    print(rows)
    menuList=[]
    for row in rows:
        print(row[0])
        menuList.append(row[0])

    conn.close()
    return render_template('store_menu.html', semail=session['semail'] , sname=sname , menuList=menuList)


@app.route("/deleteMenu", methods=['POST'])
def deleteMenu():
    print("deleteMenu func!")
    print(session['semail'])

    sname = request.form.get('sname')
    menu = request.form.get('menu')

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    #먼저 sid 구해오기
    sql = f"select distinct M.sid from store S, menu M where S.sname='{sname}' AND S.sid=M.sid"
    cur.execute(sql)
    rows = cur.fetchall()
    sid=''
    print(sid)
    for row in rows:
        print(row[0])
        sid = row[0]

    # 그다음 메뉴 삭제하기
    sql = f"delete from menu where sid='{sid}' AND menu='{menu}'"
    print(sql)
    cur.execute(sql)
    conn.commit()

    # 삭제된 메뉴 포함하는 메뉴 리스트 다시 html에 전달하여 화면에 뿌려주기
    sql = f"select M.menu from store S, menu M where S.sname='{sname}' AND S.sid=M.sid"
    print(sql)
    cur.execute(sql)
    rows = cur.fetchall()
    print(rows)
    menuList=[]
    for row in rows:
        print(row[0])
        menuList.append(row[0])

    conn.close()
    return render_template('store_menu.html', semail=session['semail'] , sname=sname , menuList=menuList)


# 판매자-가게에서 주문 관련 부분임 -> 구매자 주문 부분하고 와서 다시 하기(12/9 start!)
@app.route("/store_order_yes_no", methods=['POST'])
def store_order_yes():
    print("store_order_yes_no func!")
    print(session['semail'])

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    domain = email_Divided[1]

    sname = request.form.get('sname')

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sql = f"select S1.sid , S1.seller_id , S1.lat, S1.lng from store S1, seller S2 where S1.seller_id=S2.seller_id and S1.sname = '{sname}' and S2.local='{local}' and S2.domain='{domain}'"
    cur.execute(sql)
    rows = cur.fetchall()
    sid = -1
    seller_id = -1
    print(seller_id)
    mylat = 0.0
    mylng = 0.0
    for row in rows:
        print(row)
        sid = row[0]
        seller_id = row[1]
        mylat = row[2]
        mylng = row[3]
    # 주문 내용 가져오기
    conn.commit()
    sql = f"select menu,menu_count,phone,local,did,dname,orderstatus from orderlistforselleranddelivery where sid={sid}"
    cur.execute(sql)
    rows = cur.fetchall()
    orderList = []
    for row in rows:
        print(row)
        str_menu_count = str(row[1])
        str_did = str(row[4])
        orderList.append(row[0] + ' ' + str_menu_count + ' ' + row[2] + ' ' + row[3] + ' ' + str_did +  ' ' + row[5] + ' ' + row[6])
    
    #배달부 정보 가져오기
    conn.commit()
    sql = f"select point({mylat},{mylng}) <-> point(D.lat, D.lng) as dist, D.did, D.name, D.stock from delivery D where D.stock <= 4 order by dist limit 5"
    cur.execute(sql)
    rows = cur.fetchall()
    deliveryList = []
    for row in rows:
        print(row)
        str_dist = str(row[0])
        str_did = str(row[1])
        str_stock = str(row[3])
        deliveryList.append(str_dist + ' ' + str_did + ' ' + row[2] + ' ' + str_stock)

    conn.close()
    return render_template('store_order_yes_no.html', orderList=orderList, deliveryList=deliveryList)


@app.route("/select_store_order_yes_no", methods=['POST'])
def select_store_order_yes():
    print("select_store_order_yes_no func!")
    print(session['semail'])

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    domain = email_Divided[1]

    select = request.form.get('select')
    sname = request.form.get('sname')
    menu = request.form.get('menu')
    did = request.form.get('did')
    dname = request.form.get('dname')
    clocal = request.form.get('local')
    clocal = clocal.rstrip()
    phone = request.form.get('phone')
    phone = phone.rstrip()

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sql = f"select S1.sid , S1.seller_id , S1.lat, S1.lng from store S1, seller S2 where S1.seller_id=S2.seller_id and S1.sname = '{sname}' and S2.local='{local}' and S2.domain='{domain}'"
    cur.execute(sql)
    rows = cur.fetchall()
    sid = -1
    seller_id = -1
    print(seller_id)
    mylat = 0.0
    mylng = 0.0
    for row in rows:
        print(row)
        sid = row[0]
        seller_id = row[1]
        mylat = row[2]
        mylng = row[3]

    if select == '취소':
        sql = f"delete from orderlistforselleranddelivery where sid={sid} and sname='{sname}' and phone='{phone}' and local='{clocal}' and menu='{menu}'"
        print(sql)
        cur.execute(sql)
        conn.commit()

        conn.commit()

        sql = f"update {clocal}{phone}andcurrentorderlist set orderstatus='가게에서주문취소함' where sid={sid} and menu='{menu}'"
        print(sql)
        cur.execute(sql)
        conn.commit()

    conn.commit()
    if select == '확인':
        sql = f"update orderlistforselleranddelivery set did={did}, dname='{dname}', orderstatus='배달부할당' where seller_id={seller_id} and sid={sid} and sname='{sname}' and phone='{phone}' and menu='{menu}'"
        print(sql)
        cur.execute(sql)
        conn.commit()

        conn.commit()

        sql = f"update {clocal}{phone}andcurrentorderlist set orderstatus='가게에서주문확인함', did={did}, dname='{dname}' where sid={sid} and menu='{menu}'"
        print(sql)
        cur.execute(sql)
        conn.commit()

    # 주문 내용 가져오기
    conn.commit()
    sql = f"select menu,menu_count,phone,local,did,dname,orderstatus from orderlistforselleranddelivery where sid={sid}"
    cur.execute(sql)
    rows = cur.fetchall()
    orderList = []
    for row in rows:
        print(row)
        str_menu_count = str(row[1])
        str_did = str(row[4])
        orderList.append(row[0] + ' ' + str_menu_count + ' ' + row[2] + ' ' + row[3] + ' ' + str_did +  ' ' + row[5] + ' ' + row[6])
    
    #배달부 정보 가져오기
    conn.commit()
    sql = f"select point({mylat},{mylng}) <-> point(D.lat, D.lng) as dist, D.did, D.name, D.stock from delivery D where D.stock <= 4 order by dist limit 5"
    cur.execute(sql)
    rows = cur.fetchall()
    deliveryList = []
    for row in rows:
        print(row)
        str_dist = str(row[0])
        str_did = str(row[1])
        str_stock = str(row[3])
        deliveryList.append(str_dist + ' ' + str_did + ' ' + row[2] + ' ' + str_stock)

    conn.close()
    return render_template('store_order_yes_no.html', orderList=orderList, deliveryList=deliveryList)

# json 테스트용
@app.route("/test")
def test():
    print("test func!")
    print(session['semail'])

    conn = pg.connect(connect_string)
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)

    sql = f"select payments from customer where phone='01095434668'"
    cur.execute(sql)
    rows = cur.fetchall()
    pay = rows[0]['payments'] #그냥 pay는 JSON이라서 파이썬 에서 사용할 수 있도록 json.loads함수로 변형시켜야 함
    print(pay)
    newpay = json.loads(pay)
    print(newpay)
    print()
    for row in newpay:
       print(row)
       print(row['type']) #각 결제수단의 type을 얻어올 수 있음!
       print(row['data'])
       if(row['type'] == 'account'):
           print(row['data']["bid"])
           print(row['data']["acc_num"]) # 된다! num를 얻어올 수 있다!
       if(row['type'] == 'card'):
           print(row['data']["card_num"]) # 된다! num를 얻어올 수 있다!
    
    #ast.literal_eval(파이썬 딕셔너리 형태의 스트링) -> 파이썬 딕셔너리로 바꾸어준다!

    conn.close()
    return render_template('error.html', msg=session['semail'])


@app.route("/change_customerInfo", methods=['POST'])
def change_customerInfo():
    print("change_customerInfo func!")
    print(session['semail'])
    
    sname = request.form.get('sname')
    passwd = request.form.get('passwd')

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    domain = email_Divided[1]

    sql = f"SELECT C.phone FROM customer C WHERE C.local='{local}' AND C.domain='{domain}'"
    cur.execute(sql)
    rows = cur.fetchall()
    phone=''
    for row in rows:
        phone = row[0]
    phone=phone.rstrip()

    #주소록 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andaddressBook"
    cur.execute(sql)
    rows = cur.fetchall()

    addressBook = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        addressBook.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
    #결제수단(account) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_account"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_account = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_account.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
    #결제수단(card) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_card"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_card = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_card.append(row[0] + ' ' + row[1] )

    #이제 사전작업 끝내고 실제 내 정보 바꾸는 부분
    sql = f"UPDATE customer SET name='{sname}', passwd='{passwd}' WHERE local='{local}' AND domain='{domain}' AND phone='{phone}'"
    cur.execute(sql)
    conn.commit()

    conn.close()
    return render_template('customer.html', semail=session['semail'], addressBook=addressBook, payMethod_account=payMethod_account, payMethod_card=payMethod_card)


@app.route("/inserttoaddressBook", methods=['POST'])
def inserttoaddressBook():
    print("inserttoaddressBook func!")
    print(session['semail'])
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    phone = request.form.get('phone')
    phone=phone.rstrip()
    si = request.form.get('si')
    gu = request.form.get('gu')
    dong = request.form.get('dong')

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]
    
    sql = f"INSERT INTO {local}{phone}andaddressBook VALUES ('{si}' ,'{gu}' ,'{dong}')"
    cur.execute(sql)
    conn.commit()

    sql = f"SELECT * FROM {local}{phone}andaddressBook"
    cur.execute(sql)
    rows = cur.fetchall()
    addressBook = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        addressBook.append(row[0] + ' ' + row[1] + ' ' + row[2] )

    #결제수단(account) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_account"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_account = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_account.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
    #결제수단(card) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_card"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_card = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_card.append(row[0] + ' ' + row[1] )

    conn.close()
    return render_template('customer.html', semail=session['semail'], addressBook=addressBook, payMethod_account=payMethod_account, payMethod_card=payMethod_card)


@app.route("/changeaddressBook", methods=['POST'])
def changeaddressBook():
    print("changeaddressBook func!")
    print(session['semail'])
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    phone = request.form.get('phone')
    phone=phone.rstrip()
    si = request.form.get('si')
    gu = request.form.get('gu')
    dong = request.form.get('dong')

    old_si = request.form.get('old_si')
    old_gu = request.form.get('old_gu')
    old_dong = request.form.get('old_dong')

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]
    sql = f"UPDATE {local}{phone}andaddressBook SET si='{si}', gu='{gu}', dong='{dong}' where si='{old_si}' AND gu='{old_gu}' AND dong='{old_dong}'"
    cur.execute(sql)
    conn.commit()

    sql = f"SELECT * FROM {local}{phone}andaddressBook"
    cur.execute(sql)
    rows = cur.fetchall()
    addressBook = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        addressBook.append(row[0] + ' ' + row[1] + ' ' + row[2] )

    #결제수단(account) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_account"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_account = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_account.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
    #결제수단(card) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_card"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_card = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_card.append(row[0] + ' ' + row[1] )

    conn.close()
    return render_template('customer.html', semail=session['semail'], addressBook=addressBook, payMethod_account=payMethod_account, payMethod_card=payMethod_card)


@app.route("/removefromaddressBook", methods=['POST'])
def removefromaddressBook():
    print("removefromaddressBook func!")
    print(session['semail'])
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    phone = request.form.get('phone')
    phone=phone.rstrip()
    si = request.form.get('si')
    gu = request.form.get('gu')
    dong = request.form.get('dong')

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]
    
    sql = f"delete from {local}{phone}andaddressBook where si='{si}' AND gu='{gu}' AND dong='{dong}'"
    cur.execute(sql)
    conn.commit()

    sql = f"SELECT * FROM {local}{phone}andaddressBook"
    cur.execute(sql)
    rows = cur.fetchall()
    addressBook = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        addressBook.append(row[0] + ' ' + row[1] + ' ' + row[2] )

    #결제수단(account) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_account"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_account = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_account.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
    #결제수단(card) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_card"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_card = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_card.append(row[0] + ' ' + row[1] )

    conn.close()
    return render_template('customer.html', semail=session['semail'], addressBook=addressBook, payMethod_account=payMethod_account, payMethod_card=payMethod_card)


@app.route("/insert_change_remove_payMethod_account", methods=['POST'])
def insert_change_remove_payMethod_account():
    print("insert_change_remove_payMethod_account func!")
    print(session['semail'])
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    function = request.form.get('function')
    phone = request.form.get('phone')
    phone=phone.rstrip()
    bid = request.form.get('bid')
    acc_num = request.form.get('acc_num')
    old_bid = request.form.get('old_bid')
    old_acc_num = request.form.get('old_acc_num')

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]
    
    if function == '추가':
        sql = f"INSERT INTO {local}{phone}andpaymethod_account VALUES ('account' ,'{bid}' ,'{acc_num}')"
        cur.execute(sql)
        conn.commit()
    if function == '수정':
        sql = f"UPDATE {local}{phone}andpaymethod_account SET bid='{bid}', acc_num='{acc_num}' where bid='{old_bid}' AND acc_num='{old_acc_num}'"
        cur.execute(sql)
        conn.commit()
    if function == '삭제':
        sql = f"delete from {local}{phone}andpaymethod_account where bid='{bid}' AND acc_num='{acc_num}'"
        cur.execute(sql)
        conn.commit()

    sql = f"SELECT * FROM {local}{phone}andaddressBook"
    cur.execute(sql)
    rows = cur.fetchall()
    addressBook = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        addressBook.append(row[0] + ' ' + row[1] + ' ' + row[2] )

    #결제수단(account) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_account"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_account = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_account.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
    #결제수단(card) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_card"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_card = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_card.append(row[0] + ' ' + row[1] )

    conn.close()
    return render_template('customer.html', semail=session['semail'], addressBook=addressBook, payMethod_account=payMethod_account, payMethod_card=payMethod_card)


@app.route("/insert_change_remove_payMethod_card", methods=['POST'])
def insert_change_remove_payMethod_card():
    print("insert_change_remove_payMethod_card func!")
    print(session['semail'])
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    function = request.form.get('function')
    phone = request.form.get('phone')
    phone=phone.rstrip()
    card_num = request.form.get('card_num')
    old_card_num = request.form.get('old_card_num')

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]
    
    if function == '추가':
        sql = f"INSERT INTO {local}{phone}andpaymethod_card VALUES ('card' ,'{card_num}')"
        cur.execute(sql)
        conn.commit()
    if function == '수정':
        sql = f"UPDATE {local}{phone}andpaymethod_card SET card_num='{card_num}' where card_num='{old_card_num}'"
        cur.execute(sql)
        conn.commit()
    if function == '삭제':
        sql = f"delete from {local}{phone}andpaymethod_card where card_num='{card_num}'"
        cur.execute(sql)
        conn.commit()

    sql = f"SELECT * FROM {local}{phone}andaddressBook"
    cur.execute(sql)
    rows = cur.fetchall()
    addressBook = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        addressBook.append(row[0] + ' ' + row[1] + ' ' + row[2] )

    #결제수단(account) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_account"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_account = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_account.append(row[0] + ' ' + row[1] + ' ' + row[2] )
        
    #결제수단(card) 테이블 내용 html에 전달
    sql = f"SELECT * FROM {local}{phone}andpaymethod_card"
    cur.execute(sql)
    rows = cur.fetchall()

    payMethod_card = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        payMethod_card.append(row[0] + ' ' + row[1] )

    conn.close()
    return render_template('customer.html', semail=session['semail'], addressBook=addressBook, payMethod_account=payMethod_account, payMethod_card=payMethod_card)


@app.route("/customer_purchase", methods=['POST'])
def customer_purchase(): #storesOnSale과 storesOnSearch(이건 x 실제 검색 함수에서 전달하자) 변수 전달해야 줘야 함 
    print("customer_purchase func!")
    print(session['semail'])
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    phone = request.form.get('phone')
    phone=phone.rstrip()

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]

    try: #구매자로컬폰andcurrentshoppingbasket 테이블 생성하기
        sql = f"CREATE TABLE {local}{phone}andcurrentshoppingbasket (seller_id integer, sid integer, phone char(13), sname char(60), menu char(60), payment char(8), menu_count integer)"
        print(sql)
        cur.execute(sql)
        conn.commit()
    except:
        print(f"{local}{phone}andcurrentshoppingbasket table already exists")
    conn.commit()
    try: #구매자로컬폰andcurrentOrderList 테이블 생성하기
        sql = f"CREATE TABLE {local}{phone}andcurrentOrderList (seller_id integer, sid integer, phone char(13), sname char(60), menu char(60), payment char(8), menu_count integer, time timestamp, did integer, dname char(60), orderstatus char(20))"
        print(sql)
        cur.execute(sql)
        conn.commit()
    except:
        print(f"{local}{phone}andcurrentOrderList table already exists")

    conn.close()
    return render_template('customer_purchase.html')


@app.route("/geo", methods=['POST'])
def geo():
    return render_template('geo.html')


@app.route("/searchstoreOnsale", methods=['POST'])
def searchstoreOnsale():
    print("searchstoreOnsale func!")
    print(session['semail'])
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    mylat = request.form.get('lat')
    mylng = request.form.get('lng')

    if mylat=='customer' and mylng=='customer': #이때 구매자의 위도,경도 사용
        email_String = session['semail']
        email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
        local = email_Divided[0]
        domain = email_Divided[1]

        sql=f"select lat,lng from customer where local='{local}' and domain='{domain}'"
        cur.execute(sql)
        rows = cur.fetchall()
        for row in rows:
            print(row)
            mylat=row[0]
            mylng=row[1]

    sql=f"select point({mylat},{mylng}) <-> point(S.lat, S.lng) as dist ,S.seller_id, S.sid, S.sname from store S order by dist"
    cur.execute(sql)
    rows = cur.fetchall()

    storesOnSale = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        dist = str(row[0])
        seller_id = str(row[1])
        sid = str(row[2])
        storesOnSale.append( dist + ' ' + seller_id + ' ' + sid + ' ' + row[3] )

    conn.close()
    return render_template('searchstoreOnsale.html',storesOnSale=storesOnSale)


@app.route("/searchstore_addr_tag_storename", methods=['POST'])
def searchstore_addr_tag_storename():
    print("searchstore_addr_tag_storename func!")
    print(session['semail'])
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    function = request.form.get('function')
    addr_tag_storename = request.form.get('addr_tag_storename')

    if function == '주소':
        sql=f"select S.sname , S.address , S.sid from store S where S.address='{addr_tag_storename}'"
        cur.execute(sql)
        rows = cur.fetchall()
    if function == '태그':
        sql=f"select S2.sname , S2.address , S2.sid from sellerandstore S1, store S2 where S1.sid = S2.sid AND ( S1.tag1='{addr_tag_storename}' OR S1.tag2='{addr_tag_storename}' )" 
        cur.execute(sql)
        rows = cur.fetchall()
    if function == '가게이름':
        sql=f"select S.sname , S.address , S.sid from store S where S.sname='{addr_tag_storename}'"
        cur.execute(sql)
        rows = cur.fetchall()

    storeList = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        print(row)
        str_sid = str(row[2])
        storeList.append( row[0] + ' ' + str_sid + '   ' + row[1] )

    conn.close()
    return render_template('searchstore_addr_tag_storename.html',storeList=storeList)


@app.route("/create_shoppingbasket", methods=['POST'])
def create_shoppingbasket():
    print("create_shoppingbasket func!")
    print(session['semail'])
    
    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    phone = request.form.get('phone')
    phone = phone.rstrip()
    seller_id = request.form.get('seller_id')
    sid = request.form.get('sid')
    storename = request.form.get('storename')
    menu = request.form.get('menu')
    count = request.form.get('count')
    # form에 입력된 내용을 기반으로 실제 basket 테이블에 tuple넣기!

    #만약 이 구매자가 결제수단을 가지고 있지 않다면 에러 화면으로 ㄱㄱ
    try:
        sql = f"SELECT COUNT(*) FROM {local}{phone}andpaymethod_account, {local}{phone}andpaymethod_card"
        print(sql)
        cur.execute(sql)
        rows = cur.fetchall()
        print(rows)
        if rows == []:
            render_template('error.html',msg='no payments')
    except:
        return render_template('error.html',msg='no table, no payments')
    
    
    sql = f"insert into {local}{phone}andcurrentshoppingbasket values ({seller_id}, {sid}, '{phone}', '{storename}', '{menu}', ' ', {count}) "
    cur.execute(sql)
    conn.commit()

    sql = f"select seller_id, sid, sname, menu, menu_count from {local}{phone}andcurrentshoppingbasket"
    cur.execute(sql)
    rows = cur.fetchall()
    shoppingbasket = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        print(row)
        str_seller_id = str(row[0])
        str_sid = str(row[1])
        count = str(row[4])
        shoppingbasket.append( str_seller_id + ' ' + str_sid + ' ' +  row[2] + '  ' + row[3] + ' ' + count)
    
    conn.close()
    return render_template('shoppingbasketandpurchase.html',shoppingbasket=shoppingbasket)

#아래 함수가 장바구니에 있는 거 실제 주문
@app.route("/purchaseInshoppingbasket", methods=['POST'])
def purchaseInshoppingbasket():
    print("purchaseInshoppingbasket func!")
    print(session['semail'])
    
    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    now = datetime.now()
    timestring = ""
    timestring = '%s-%s-%s %s:%s:%s' % (now.year, now.month, now.day, now.hour, now.minute, now.second)

    payment = request.form.get('payment')
    phone = request.form.get('phone')
    phone = phone.rstrip()
    seller_id = request.form.get('seller_id')
    sid = request.form.get('sid')
    storename = request.form.get('storename')
    menu = request.form.get('menu')
    count = request.form.get('count')
    # form에 입력된 내용을 기반으로 실제 basket 테이블에서 지우고 order 테이블에 넣기!
    sql = f"insert into {local}{phone}andcurrentOrderList values ({seller_id}, {sid}, '{phone}', '{storename}', '{menu}', '{payment}', {count}, '{timestring}', -1 , '아직없음', '아직안정해짐')"
    cur.execute(sql)
    conn.commit()
    sql = f"delete from {local}{phone}andcurrentshoppingbasket where phone='{phone}' AND sname='{storename}' AND menu='{menu}' AND menu_count='{count}'"
    cur.execute(sql)
    conn.commit()

    #결제시 orderlistforselleranddelivery 테이블 생성
    try:
        conn.commit()
        sql = f"create table orderlistforselleranddelivery (seller_id integer, sid integer, phone char(13), local char(15) , sname char(60), menu char(60), menu_count integer, did integer, dname char(60), orderstatus char(20))"
        print(sql)
        cur.execute(sql)
        conn.commit()
    except:
        print("orderlistforselleranddelivery already exists")
    conn.commit()
    sql = f"insert into orderlistforselleranddelivery values ({seller_id}, {sid}, '{phone}', '{local}' , '{storename}', '{menu}', {count}, -1 , '배달부' ,'아직없음')"
    cur.execute(sql)
    conn.commit()

    sql = f"select seller_id, sid, sname, menu, menu_count from {local}{phone}andcurrentshoppingbasket"
    cur.execute(sql)
    rows = cur.fetchall()
    shoppingbasket = []
    for row in rows: #row가 tuple -> () 이라 하나의 문자열만 뽑아내기 위해 index사용
        print(row)
        str_seller_id = str(row[0])
        str_sid = str(row[1])
        count = str(row[4])
        shoppingbasket.append( str_seller_id + ' ' + str_sid + ' ' +  row[2] + '  ' + row[3] + ' ' + count)
    
    conn.close()
    return render_template('shoppingbasketandpurchase.html',shoppingbasket=shoppingbasket)


#customer_order
@app.route("/customer_order", methods=['POST'])
def customer_order():
    print("customer_order func!")
    print(session['semail'])

    phone = request.form.get('phone')
    phone=phone.rstrip()

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]

    conn = pg.connect(connect_string)
    cur = conn.cursor()
    #주문 상태, 주문한 가게, 메뉴, 결제수단, 주문 시간, did, dname
    sql = f"select orderstatus, sname, menu, payment, TEXT(time), did, dname from {local}{phone}andcurrentOrderList"
    cur.execute(sql)
    rows = cur.fetchall()
    orderList = []
    for row in rows:
        print(row)
        str_did = str(row[5])
        print(row[4])
        orderList.append(row[0] + ' ' + row[1] + ' ' + row[2] + ' ' + row[3] + ' ' + row[4] + ' ' + str_did + ' ' + row[6])

    conn.close()
    return render_template('customer_order.html',orderList=orderList)


@app.route("/move_currentorder_to_orderhistory", methods=['POST'])
def move_currentorder_to_orderhistory():
    print("move_currentorder_to_orderhistory func!")
    print(session['semail'])

    phone = request.form.get('phone')
    phone=phone.rstrip()
    storename = request.form.get('storename')
    menu = request.form.get('menu')
    payment = request.form.get('payment')

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]

    conn = pg.connect(connect_string)
    cur = conn.cursor()
    
    # {local}{phone}andOrderHistory 테이블 없으면 여기서 생성 후 해당 tuple 넣어주기
    try:
        sql = f"CREATE TABLE {local}{phone}andOrderHistory (sname char(60), menu char(60), payment char(8), time timestamp)"
        print(sql)
        cur.execute(sql)
        conn.commit()
    except:
        print(f"{local}{phone}andOrderHistory table already exists")
    conn.commit()

    sql = f"select time from {local}{phone}andcurrentOrderList where sname='{storename}' AND menu='{menu}' AND payment='{payment}'"
    cur.execute(sql)
    rows = cur.fetchall()
    time =''
    for row in rows:
        print(row[0])
        time = row[0]

    conn.commit()
    sql = f"insert into {local}{phone}andOrderHistory values ('{storename}', '{menu}', '{payment}', '{time}')"
    cur.execute(sql)
    conn.commit()

    # 해당 tuple 지우기
    sql = f"delete from {local}{phone}andcurrentOrderList where sname='{storename}' AND menu='{menu}' AND payment='{payment}'"
    cur.execute(sql)
    conn.commit()

    conn.commit()
    sql = f"select orderstatus, sname, menu, payment, TEXT(time), did, dname from {local}{phone}andcurrentOrderList"
    cur.execute(sql)
    rows = cur.fetchall()
    orderList = []
    for row in rows:
        str_did = str(row[5])
        orderList.append(row[0] + ' ' + row[1] + ' ' + row[2] + ' ' + row[3] + ' ' + row[4] + ' ' + str_did + ' ' + row[6])

    conn.close()
    return render_template('customer_order.html',orderList=orderList)

#sql = f"insert into {local}{phone}andcurrentOrderList values ({seller_id}, {sid}, '{phone}', '{storename}', '{menu}', '{payment}', {count}, '{timestring}',
# 1구매자 주문 내역 2판매자 주문 부분 합시다



@app.route("/orderhistory", methods=['POST'])
def orderhistory():
    print("orderhistory func!")
    print(session['semail'])

    phone = request.form.get('phone')
    phone=phone.rstrip()

    email_String = session['semail']
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    local = email_Divided[0]
    #domain = email_Divided[1]

    conn = pg.connect(connect_string)
    cur = conn.cursor()
    #주문한 가게, 메뉴, 결제수단, 주문 시간
    sql = f"select sname,menu,payment,TEXT(time) from {local}{phone}andOrderHistory order by time"
    cur.execute(sql)
    rows = cur.fetchall()
    orderhistory = []
    for row in rows:
        print(row)
        orderhistory.append(row[0] + ' ' + row[1] + ' ' + row[2] + ' ' + row[3] )

    conn.close()
    return render_template('orderhistory.html',orderhistory=orderhistory)


@app.route("/orders_in_delivery", methods=['POST'])
def orders_in_delivery():
    print("orders_in_delivery func!")
    print(session['semail'])

    did = request.form.get('did')
    dname = request.form.get('dname')

    #email_String = session['semail']
    #email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    #local = email_Divided[0]
    #domain = email_Divided[1]

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    #가게이름, 구매자 phone, 메뉴, 주문시간 뿌려줘야 함
    sql = f"select D.sname,D.phone,D.menu from orderlistforselleranddelivery D where D.did={did} and D.dname='{dname}' and D.orderstatus='배달부할당'"
    cur.execute(sql)
    rows = cur.fetchall()
    
    sql = f"select phone,local from orderlistforselleranddelivery where did={did}"
    cur.execute(sql)
    rows2 = cur.fetchall()
    phone = ''
    local = ''
    for row in rows2:
        print(row)
        phone = row[0]
        phone = phone.rstrip()
        local = row[1]
        local = local.rstrip()
    
    sql = f"select TEXT(time) from {local}{phone}andcurrentorderlist where did={did} and dname='{dname}' and orderstatus='가게에서주문확인함'"
    print(sql)
    cur.execute(sql)
    rows3 = cur.fetchall()
    timeval = ['','','','','','','','','']
    i=0
    for timerow in rows3:
        timeval[i] = timerow[0]
        i+=1

    i=0

    orderIndelivery = []
    if rows3:
        for row in rows:
            print(row)
            orderIndelivery.append(row[0] + ' ' + row[1] + ' ' + row[2] + ' ' + str(timeval[i]) )
            i+=1
    '''
    #주문 시간만 뿌려주면 된다 phone과 local은 orderlistforselleranddelivery로 구할 수 있고(이때 row의 개수 세자 그만큼 반복하도록)
    #select TEXT(time) from {phone}{local}andcurrentorderlist where did={did} and dname='{dname}' and orderstatus='{가게에서주문확인함}'
    #으로 time들 구할 수 있음!
    # 단, 구매자가 여려명일 수 있으니 phone과 local 구하는 반복문이 있어야 할 것이다;;
    sql = f"select phone,local from orderlistforselleranddelivery where did={did}"
    cur.execute(sql)
    rows = cur.fetchall()
    timesofcustomers = []
    phone = ''
    local = ''
    for row in rows:
        print(row)
        phone = row[0]
        phone = phone.rstrip()
        local = row[1]
        local = local.rstrip()
        sql = f"select TEXT(time) from {local}{phone}andcurrentorderlist where did={did} and dname='{dname}' and orderstatus='가게에서주문확인함'"
        print(sql)
        cur.execute(sql)
        rows2 = cur.fetchall()
        if not rows2:
            timesofcustomers.append("위의 tuple들은 배송완료")
        for row2 in rows2:
            print('debug1')
            print(row2)
            if not row2:
                print('debug2')
                timesofcustomers.append("이 라인과 매칭되는 위의 tuple은 배송완료")
                continue
            timesofcustomers.append(row2[0])
    '''
    conn.close()
    return render_template('display_orders_in_delivery.html',orderIndelivery=orderIndelivery)