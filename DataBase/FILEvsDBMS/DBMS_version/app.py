from flask import Flask, render_template, redirect, request
import psycopg2 as pg
import psycopg2.extras
import csv
from collections import Counter

app = Flask(__name__)

session = {'semail' : None, 'passwd' : None}
semail = ''
passwd = ''

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

    try:
        sql = "CREATE TABLE addressBook (sname CHAR(15), sid CHAR(15), phone CHAR(11), email VARCHAR(35))"
        cur.execute(sql)
        conn.commit()

        set_addressBook()
    except:
        print('index func error! or addressBook already exists')
    
    conn.close()
    return render_template("index.html")


@app.route("/login", methods=['POST'])
def login():
    semail = request.form.get('semail')
    passwd = request.form.get('passwd')

    conn = pg.connect(connect_string)
    cur = conn.cursor()
    
    if semail == 'admin@hanyang.ac.kr' and passwd == 'root':
        try:
            sql = "SELECT * FROM addressBook"
            cur.execute(sql)
            rows = cur.fetchall()
            addressBook = []
            for row in rows:
                addressBook.append(row)
        except:
            print('login func(admin) error!')
        
        session['semail'] = semail
        session['passwd'] = passwd

        conn.close()
        return render_template("adminportal.html", session=session, addressBook = addressBook)

    print(f"{semail}, {passwd}")
    try:
        sql = f"SELECT C.email, S.password FROM students S, contacts C WHERE C.email='{semail}' AND S.password='{passwd}'"
        cur.execute(sql)
        rows = cur.fetchall()
    
        if(len(rows) != 1): # 당연히 1개의 tuple만 return 되어야 한다
            return render_template('error.html', msg="Wrong ID OR Wrong PASSWORD")
    except:
        print('login func error!')
    
    conn.close()
    
    session['semail'] = semail
    session['passwd'] = passwd
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()
    
    print(semail)
    
    sql = "SELECT * FROM addressBook"
    cur.execute(sql)
    rows = cur.fetchall()
    addressBook = []
    for row in rows:
        addressBook.append(row)
    
    email_String = semail
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    semail_id = email_Divided[0] #앞에 있는 id만 사용 

    try: #개인주소록 생성 #테이블명에 @들어갈 수 없음 실수주의!
        sql = f"CREATE TABLE {semail_id} (sname CHAR(15), phone CHAR(13), email VARCHAR(35), position VARCHAR(15))"
        print(sql)
        cur.execute(sql)
        conn.commit()
    except:
        print('privateBook already exists')

    conn.close()

    conn = pg.connect(connect_string)
    cur = conn.cursor()
    
    try:
        privateBook = []
        sql = f"SELECT * FROM {semail_id}"
        print(sql)
        cur.execute(sql)
        rows = cur.fetchall()
        for row in rows:
            privateBook.append(row)
    except:
        print('login func(user) error!')
    
    for row in privateBook:
        print(row)

    conn.close()
    return render_template("portal.html", session=session, addressBook = addressBook, privateBook = privateBook)

@app.route("/<semail>/insertStudent", methods=['POST'])
def insertStudent(semail):
    session['semail'] = semail
    session['passwd'] = passwd

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    #sid,password,sname,sex,major_id,tutor_id,grade
    sid = request.form.get('sid')
    password = request.form.get('password')
    sname = request.form.get('sname')
    sex = request.form.get('sex')
    major_id = request.form.get('major_id')
    tutor_id = request.form.get('tutor_id')
    grade = request.form.get('grade')

    sql = "SELECT * FROM addressBook"
    cur.execute(sql)
    rows = cur.fetchall()
    addressBook = []
    for row in rows:
        addressBook.append(row)

    try:
        sql = f"INSERT INTO students VALUES ('{sid}','{password}','{sname}','{sex}',{major_id},'{tutor_id}',{grade})"
        cur.execute(sql)
    except:
        print('insertStudent func error!')
    conn.commit()
    conn.close()
    return render_template("adminportal.html", session=session, addressBook = addressBook)

@app.route("/<semail>/insert", methods=['POST'])
def insert(semail):
    session['semail'] = semail
    session['passwd'] = passwd
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sname = request.form.get('sname')
    sid = request.form.get('sid')
    phone = request.form.get('phone')
    email = request.form.get('email')

    if session['semail'] == 'admin@hanyang.ac.kr':
        try:
            sql = f"INSERT INTO addressBook VALUES ('{sname}' ,'{sid}' ,'{phone}' , '{email}')"
            cur.execute(sql)
        except:
            print('insert func error!')
        
        conn.commit()

        sql = "SELECT * FROM addressBook"
        cur.execute(sql)
        rows = cur.fetchall()
        addressBook = []
        for row in rows:
            addressBook.append(row)

        conn.close()
        return render_template("adminportal.html", session=session, addressBook = addressBook)
    
    #아래는 관리자가 아닐 경우

@app.route("/<semail>/insertCorp", methods=['POST'])
def insertCorp(semail):
    session['semail'] = semail
    session['passwd'] = passwd

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sql = "SELECT * FROM addressBook"
    cur.execute(sql)
    rows = cur.fetchall()
    addressBook = []
    for row in rows:
        addressBook.append(row)

    name_corp = request.form.get('name_corp')
    file_path = '{}.csv'.format(name_corp)
    read_file = open(file_path, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    people_corp = []
    for row in reader:
        people_corp.append(row)
    read_file.close()
    
    email_String = semail
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    semail_id = email_Divided[0] #앞에 있는 id만 사용 

    privateBook = []
    try:
        for row in people_corp:
            sql = f"INSERT INTO {semail_id} VALUES ('{row[0]}', '{row[1]}', '{row[2]}', '{row[3]}')"
            cur.execute(sql)
            conn.commit()
        
        sql = f"SELECT * FROM {semail_id}"
        cur.execute(sql)
        rows = cur.fetchall()
        
        for row in rows:
            privateBook.append(row)
    except:
        print('insertCorp error!')

    conn.close()
    return render_template("portal.html", session=session, addressBook = addressBook, privateBook = privateBook)


@app.route("/<semail>/change", methods=['POST'])
def change(semail):
    session['semail'] = semail
    session['passwd'] = passwd
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sname = request.form.get('sname')
    sid = request.form.get('sid')
    phone = request.form.get('phone')
    email = request.form.get('email')

    if session['semail'] == 'admin@hanyang.ac.kr':
        try:
            sql = f"UPDATE addressBook SET sname='{sname}', phone='{phone}', email='{email}' WHERE sid='{sid}'"
            cur.execute(sql)
            conn.commit()
            sql = "SELECT * FROM addressBook"
            cur.execute(sql)
            rows = cur.fetchall()
            addressBook = []
            for row in rows:
                addressBook.append(row)
        except:
            print('change func error!')

        conn.close()
        return render_template("adminportal.html", session=session, addressBook = addressBook)
    
    #아래는 관리자가 아닐 경우

@app.route("/<semail>/changeCorp", methods=['POST'])
def changeCorp(semail):
    session['semail'] = semail
    session['passwd'] = passwd

    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sql = "SELECT * FROM addressBook"
    cur.execute(sql)
    rows = cur.fetchall()
    addressBook = []
    for row in rows:
        addressBook.append(row)

    new_name_corp = request.form.get('name_corp')
    file_path = '{}.csv'.format(new_name_corp)
    read_file = open(file_path, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    new_people_corp = []
    for row in reader:
        new_people_corp.append(row)
    read_file.close()

    email_String = semail
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    semail_id = email_Divided[0] #앞에 있는 id만 사용 

    try:
        sql = f"DELETE FROM {semail_id}"
        cur.execute(sql)
        conn.commit()

        for row in new_people_corp:
            sql = f"INSERT INTO {semail_id} VALUES ('{row[0]}', '{row[1]}', '{row[2]}', '{row[3]}')"
            cur.execute(sql)

        conn.commit()
        sql = f"SELECT * FROM {semail_id}"
        cur.execute(sql)
        rows = cur.fetchall()
        privateBook = []
        for row in rows:
            privateBook.append(row)
    except:
        print('changeCorp error!')

    conn.close()
    return render_template("portal.html", session=session, addressBook = addressBook, privateBook = privateBook)

@app.route("/<semail>/remove", methods=['POST'])
def remove(semail):
    session['semail'] = semail
    session['passwd'] = passwd
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sname = request.form.get('sname')
    sid = request.form.get('sid')

    if session['semail'] == 'admin@hanyang.ac.kr':
        try:
            sql = f"UPDATE addressBook SET phone='', email='' WHERE sid='{sid}' AND sname='{sname}'"
            cur.execute(sql)
            conn.commit()
            sql = "SELECT * FROM addressBook"
            cur.execute(sql)
            rows = cur.fetchall()
            addressBook = []
            for row in rows:
                addressBook.append(row)
        except:
            print("remove func error!")

        conn.close()
        return render_template("adminportal.html", session=session, addressBook = addressBook)
    
    #아래는 관리자가 아닐 경우

@app.route("/<semail>/removeEmp", methods=['POST'])
def removeEmp(semail):
    session['semail'] = semail
    session['passwd'] = passwd
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sname = request.form.get('sname')

    sql = "SELECT * FROM addressBook"
    cur.execute(sql)
    rows = cur.fetchall()
    addressBook = []
    for row in rows:
        addressBook.append(row)
    
    email_String = semail
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    semail_id = email_Divided[0] #앞에 있는 id만 사용

    try:
        sql = f"DELETE FROM {semail_id} WHERE sname='{sname}'"
        cur.execute(sql)
        conn.commit()

        sql = f"SELECT * FROM {semail_id}"
        cur.execute(sql)
        rows = cur.fetchall()
        privateBook = []
        for row in rows:
            privateBook.append(row)
    except:
        print("removeEmp func error")

    return render_template("portal.html", session=session, addressBook = addressBook, privateBook = privateBook)

@app.route("/countPerDomain")
def countPerDomain():
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    try:
        sql = "SELECT * FROM addressBook"
        cur.execute(sql)
        rows = cur.fetchall()
        emailList_addressBook = []
        for row in rows:
            emailList_addressBook.append(row[3])
    except:
        print('countPerDomain error1 !!!')

    local_part = []
    domain_name = []
    try:
        for row in emailList_addressBook:
            if row == '': #삭제된 이메일은 뛰어넘어야 함
                continue

            email_String = row
            email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
            local_part.append(email_Divided[0])
            domain_name.append(email_Divided[1]) #뒤에 있는 domain_name만 사용 
    except:
        print('countPerDomain error2 !!!')
    
    countTable = []
    result = Counter(domain_name) #domain_name은 주소록에 있는 domain들의 리스트
    for key in result:
        countTable.append( [key,result[key]] ) #key가 이메일. result의 key번째 요소가 빈도수

    sql = f"DELETE FROM localpart"
    cur.execute(sql)
    conn.commit()
    for row in local_part:
        sql = f"INSERT INTO localpart VALUES ('{row}')"
        cur.execute(sql)
        conn.commit()
    
    sql = f"DELETE FROM domainname"
    cur.execute(sql)
    conn.commit()
    for row in domain_name:
        sql = f"INSERT INTO domainname VALUES ('{row}')"
        cur.execute(sql)
        conn.commit()

    conn.close()
    return render_template("adminportal_countTable.html", countTable = countTable)

#db에 commit을 해줘야 실제 원격 db서버로 tuple 리스트들이 들어감
@app.route("/privateBook_and_countPerDomain", methods=['POST'])
def privateBook_and_countPerDomain():
    session['semail'] = semail
    session['passwd'] = passwd
    
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    email = request.form.get('email')

    email_String = email
    email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
    semail_id = email_Divided[0] #앞에 있는 id만 사용

    privateBook = []
    emailList_privateBook = []

    try:
        sql = f"SELECT * FROM {semail_id}"
        cur.execute(sql)
        rows = cur.fetchall()
        
        for row in rows:
            privateBook.append(row)
            emailList_privateBook.append(row[2])
    except:
        print('privateBook_and_countPerDomain error1 !!!')

    local_part = []
    domain_name = []

    for row in emailList_privateBook:
            email_String = row
            email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
            local_part.append(email_Divided[0])
            domain_name.append(email_Divided[1]) #뒤에 있는 domain_name만 사용 
    
    countTable = []
    result = Counter(domain_name)
    for key in result:
        countTable.append( [key,result[key]] )

    conn.close()
    return render_template("privatePortal_countTable.html", privateBook = privateBook , email=email , countTable = countTable)

def set_addressBook():
    conn = pg.connect(connect_string)
    cur = conn.cursor()

    sql = "SELECT * FROM contacts"
    cur.execute(sql)
    rows_contacts = cur.fetchall()

    sql = "SELECT * FROM students"
    cur.execute(sql)
    rows_students = cur.fetchall()
    
    try:
        for row_contacts in rows_contacts:
            for row_students in rows_students:
                if(row_students[0] == row_contacts[0]):
                    # 실수 주의 field들 string으로 넣어주어야 함
                    sql = f"INSERT INTO addressBook VALUES ('{row_students[2]}', '{row_students[0]}', '{row_contacts[1]}', '{row_contacts[2]}')"
                    cur.execute(sql)
                    conn.commit()
                    break
    except:
        print("INSERT INTO addressBook error")
        return -1

    sql = "CREATE TABLE localpart (local_part CHAR(30))"
    cur.execute(sql)
    conn.commit()

    sql = "CREATE TABLE domainname (domain_name CHAR(30))"
    cur.execute(sql)
    conn.commit()

    conn.close()
    return 0

if __name__ == "__main__":
    app.run(debug=True)

