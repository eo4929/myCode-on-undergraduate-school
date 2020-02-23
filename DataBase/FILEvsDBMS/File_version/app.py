from flask import Flask, render_template, redirect, request
import csv
from collections import Counter

app = Flask(__name__)

session = {'semail' : None, 'passwd' : None}
semail = ''
passwd = ''

csv1 = []
csv2 = []
newCsv = []

@app.route("/")
def index():
    csv1 = get_csv('contacts.csv')
    csv2 = get_csv('students.csv')
    newCsv = get_new_csv(csv1,csv2)
    #for row in newCsv: #csv 내용 한 list씩 출력
    #    print(row)      # 주의! 이 함수 벗어나면 newCsv는 그대로 [] 상태가 됨;;

    return render_template("index.html", newCsv = newCsv)

@app.route('/login', methods=['POST'])
def login():
    semail = request.form.get('semail')
    passwd = request.form.get('passwd')
    sid_check =''

    if(semail == 'admin@hanyang.ac.kr' and passwd == 'root'):
        return redirect(f"/{semail}")

    print(f"{semail}, {passwd}")

    csv2 = get_csv('students.csv')
    read_file = open('addressBook.csv',encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    newCsv = []
    for row in reader:
        newCsv.append(row)
    read_file.close()

    for row in csv2: #동일한 패스워드는 없으니 passwd로 students의 sid 뽑음
        stuPasswd = row[1].rstrip() #실수 주의! 오른쪽 공백을 다 자르고 비교해야 된다!
        if passwd == stuPasswd:
            print(stuPasswd)
            sid_check = row[0]
            print(sid_check)
            break

    line_count = 0
    exist_Email = False
    for row in newCsv:
        if(line_count >= 1):
            if(row[3] == semail and sid_check == row[1]):
                exist_Email = True
                break

        line_count+=1

    if(exist_Email == False):
        return render_template('error.html', msg="Wrong ID or PASSWORD")
    
    return redirect(f"/{semail}")

@app.route("/<semail>")
def portal(semail):
    session['semail'] = semail
    session['passwd'] = passwd
    
    if semail == 'admin@hanyang.ac.kr':
        filename = 'addressBook.csv'
        file_path = '{}'.format(filename)
        read_file = open(file_path, encoding='utf-8')
        reader = csv.reader(read_file, delimiter=',')
        newCsv = []
        for row in reader:
            newCsv.append(row)

        read_file.close()
        return render_template("adminportal.html", session=session, newCsv = newCsv)
    
    #일반 사용자
    filename = 'addressBook.csv'
    file_path = '{}'.format(filename)
    read_file = open(file_path, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    newCsv = []
    for row in reader:
        newCsv.append(row)
    read_file.close()
    
    try:
        f = open(f'{semail}.csv', 'r', encoding='utf-8') #아직 개인주소록이 생성되지 않았다면
        f.close()
    except:
        filename2 = f'{semail}.csv'
        file_path2 = '{}'.format(filename2)
        f = open(file_path2, 'w', encoding='utf-8', newline='') #개인주소록 생성
        wr = csv.writer(f)
        wr.writerow(['sname','phone','email','position'])
        f.close()

    filename2 = f'{semail}.csv'
    file_path2 = '{}'.format(filename2)
    read_file = open(file_path2, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    privateCsv = []
    for row in reader:
        privateCsv.append(row)
    read_file.close()

    return render_template("portal.html", session=session, newCsv = newCsv, privateCsv = privateCsv)

@app.route("/<semail>/insertStudent", methods=['POST'])
def insertStudent(semail):
    session['semail'] = semail
    session['passwd'] = passwd

    read_file = open('addressBook.csv', encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    newCsv = []
    for row in reader:
        newCsv.append(row)

    #sid,password,sname,sex,major_id,tutor_id,grade
    sid = request.form.get('sid')
    while len(sid) < 15:
        sid += ' '
    password = request.form.get('password')
    while len(password) < 15:
        password += ' '
    sname = request.form.get('sname')
    sex = request.form.get('sex')
    if sex == 'male':
        sex += '  '
    major_id = request.form.get('major_id')
    tutor_id = request.form.get('tutor_id')
    while len(tutor_id) < 15:
        tutor_id += ' '
    grade = request.form.get('grade')

    filename = 'students.csv'
    file_path = '{}'.format(filename)

    f = open(file_path, 'a', encoding='utf-8', newline='')
    wr = csv.writer(f)
    wr.writerow([sid, password, sname, sex, major_id, tutor_id, grade])
    f.close()

    return render_template("adminportal.html", session=session, newCsv = newCsv)

@app.route("/<semail>/insert", methods=['POST'])
def insert(semail):
    session['semail'] = semail
    session['passwd'] = passwd
    
    sname = request.form.get('sname')
    sid = request.form.get('sid')
    while len(sid) < 15:
        sid += ' '
    phone = request.form.get('phone')
    email = request.form.get('email')

    if session['semail'] == 'admin@hanyang.ac.kr':
        filename = 'addressBook.csv'
        file_path = '{}'.format(filename)

        f = open(file_path, 'a', encoding='utf-8', newline='')
        wr = csv.writer(f)
        wr.writerow([sname, sid, phone, email])
        f.close()

        read_file = open(file_path, encoding='utf-8')
        reader = csv.reader(read_file, delimiter=',')
        newCsv = []
        for row in reader:
            newCsv.append(row)

        read_file.close()
        return render_template("adminportal.html", session=session, newCsv = newCsv)
    
    #아래는 관리자가 아닐 경우

@app.route("/<semail>/insertCorp", methods=['POST'])
def insertCorp(semail):
    session['semail'] = semail
    session['passwd'] = passwd

    read_file = open('addressBook.csv', encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    newCsv = []
    for row in reader:
        newCsv.append(row)
    read_file.close()

    name_corp = request.form.get('name_corp')
    file_path = '{}.csv'.format(name_corp)
    read_file = open(file_path, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    people_corp = []
    for row in reader:
        people_corp.append(row)
    read_file.close()

    filename2 = f'{semail}.csv'
    file_path2 = '{}'.format(filename2)
    f = open(file_path2, 'a', encoding='utf-8', newline='')
    wr = csv.writer(f)
    for row in people_corp:
        wr.writerow([row[0], row[1], row[2], row[3]])
    f.close()

    filename2 = f'{semail}.csv'
    file_path2 = '{}'.format(filename2)
    read_file = open(file_path2, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    privateCsv = []
    for row in reader:
        privateCsv.append(row)
    read_file.close()

    return render_template("portal.html", session=session, newCsv = newCsv, privateCsv = privateCsv)


@app.route("/<semail>/change", methods=['POST'])
def change(semail):
    session['semail'] = semail
    session['passwd'] = passwd
    
    sname = request.form.get('sname')
    sid = request.form.get('sid')
    phone = request.form.get('phone')
    email = request.form.get('email')

    if session['semail'] == 'admin@hanyang.ac.kr':
        filename = 'addressBook.csv'
        file_path = '{}'.format(filename)

        read_file = open(file_path, encoding='utf-8')
        reader = csv.reader(read_file, delimiter=',')
        newCsv = []
        for row in reader:
            newCsv.append(row)
        read_file.close()

        for row in newCsv:
            stusid = row[1].rstrip()
            if(sid == stusid):
                row[0] = sname
                row[1] = sid
                while len(row[1]) < 15:
                    row[1] += ' '
                row[2] = phone
                row[3] = email

        f = open(file_path, 'w', encoding='utf-8', newline='')
        wr = csv.writer(f)
        for row in newCsv:
            wr.writerow([row[0], row[1], row[2], row[3]])
        f.close()

        read_file = open(file_path, encoding='utf-8')
        reader = csv.reader(read_file, delimiter=',')
        newCsv = []
        for row in reader:
            newCsv.append(row)

        read_file.close()
        return render_template("adminportal.html", session=session, newCsv = newCsv)
    
    #아래는 관리자가 아닐 경우

@app.route("/<semail>/changeCorp", methods=['POST'])
def changeCorp(semail):
    session['semail'] = semail
    session['passwd'] = passwd

    read_file = open('addressBook.csv', encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    newCsv = []
    for row in reader:
        newCsv.append(row)
    read_file.close()

    new_name_corp = request.form.get('name_corp')
    file_path = '{}.csv'.format(new_name_corp)
    read_file = open(file_path, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    new_people_corp = []
    for row in reader:
        new_people_corp.append(row)
    read_file.close()

    filename2 = f'{semail}.csv'
    file_path2 = '{}'.format(filename2)
    read_file = open(file_path2, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    privateCsv = []
    for row in reader:
        privateCsv.append(row)
    read_file.close()

    for row1 in privateCsv:
        for row2 in new_people_corp:
            if row2[1] == row1[1]: #업데이트되는 정보에 전화번호를 제외된다고 함
                row1[0] = row2[0] #즉, 전화번호가 같으면 동일인 취급
                row1[2] = row2[2]
                row1[3] = row2[3]
    phoneList =[]
    for row in privateCsv: #개인주소록의 전화번호로 리스트 생성
        phoneList.append(row[1])

    for row in new_people_corp:
         if row[1] not in phoneList: #개인주소록에 없는 전화번호에 대한 연락처 정보는 추가
             privateCsv.append(row)

    newphoneList = []
    for row in new_people_corp: #업데이트할 주소록의 전화번호들
        newphoneList.append(row[1])

    f = open(file_path2, 'w', encoding='utf-8', newline='')
    wr = csv.writer(f)
    for row in privateCsv:
        if row[1] in newphoneList:
            print(row[1])
            wr.writerow([row[0], row[1], row[2], row[3]])
    f.close()

    read_file = open(file_path2, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    privateCsv = []
    for row in reader:
        privateCsv.append(row)
    read_file.close()

    return render_template("portal.html", session=session, newCsv = newCsv, privateCsv = privateCsv)

@app.route("/<semail>/remove", methods=['POST'])
def remove(semail):
    session['semail'] = semail
    session['passwd'] = passwd
    
    sname = request.form.get('sname')
    sid = request.form.get('sid')

    if session['semail'] == 'admin@hanyang.ac.kr':
        filename = 'addressBook.csv'
        file_path = '{}'.format(filename)

        read_file = open(file_path, encoding='utf-8')
        reader = csv.reader(read_file, delimiter=',')
        newCsv = []
        for row in reader:
            newCsv.append(row)
        read_file.close()

        for row in newCsv:
            stusid = row[1].rstrip()
            if(sid == stusid and sname == row[0]):
                row[0] = sname
                row[1] = sid
                row[2] = ''
                row[3] = ''

        f = open(file_path, 'w', encoding='utf-8', newline='')
        wr = csv.writer(f)
        for row in newCsv:
            wr.writerow([row[0], row[1], row[2], row[3]])
        f.close()

        read_file = open(file_path, encoding='utf-8')
        reader = csv.reader(read_file, delimiter=',')
        newCsv = []
        for row in reader:
            newCsv.append(row)

        read_file.close()
        return render_template("adminportal.html", session=session, newCsv = newCsv)
    
    #아래는 관리자가 아닐 경우

@app.route("/<semail>/removeEmp", methods=['POST'])
def removeEmp(semail):
    session['semail'] = semail
    session['passwd'] = passwd
    
    phone = request.form.get('phone')

    read_file = open('addressBook.csv', encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    newCsv = []
    for row in reader:
        newCsv.append(row)
    read_file.close()

    filename2 = f'{semail}.csv'
    file_path2 = '{}'.format(filename2)
    read_file = open(file_path2, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    privateCsv = []
    for row in reader:
        privateCsv.append(row)
    read_file.close()

    for row in privateCsv:
        rowphone = row[1].lstrip() #실수 주의! 왼쪽 공백을 다 자르고 비교해야 된다!
        if phone == rowphone:
            print('detect!!!')
            row[0] = ''
            row[1] = ''
            row[2] = ''
            row[3] = ''

    newPrivateCsv = []
    for row in privateCsv:
        if row[1] != '':
            newPrivateCsv.append(row)

    f = open(file_path2, 'w', encoding='utf-8', newline='')
    wr = csv.writer(f)
    for row in newPrivateCsv:
        wr.writerow([row[0], row[1], row[2], row[3]])
    f.close()

    return render_template("portal.html", session=session, newCsv = newCsv, privateCsv = newPrivateCsv)

@app.route("/countPerDomain")
def countPerDomain():
    filename = 'addressBook.csv'
    file_path = '{}'.format(filename)

    read_file = open(file_path, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    newEmailCsv = []
    for row in reader:
        newEmailCsv.append(row[3])
    read_file.close()

    local_part = []
    domain_name = []

    tuple_count=0
    for row in newEmailCsv:
        if row == '': #삭제된 이메일은 뛰어넘어야 함
            continue
        if tuple_count >=1:
            email_String = row
            email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
            local_part.append(email_Divided[0])
            domain_name.append(email_Divided[1]) #뒤에 있는 domain_name만 사용 
        tuple_count+=1
    
    countTable = []
    result = Counter(domain_name)
    for key in result:
        countTable.append( [key,result[key]] )
    
    f = open('local_part.csv', 'w', encoding='utf-8', newline='')
    wr = csv.writer(f)
    wr.writerow(['localpart'])
    for row in local_part:
        wr.writerow([row]) #실수 주의 [ ] 즉, 리스트로 넘겨줘야 함
    f.close()

    f = open('domain_name.csv', 'w', encoding='utf-8', newline='')
    wr = csv.writer(f)
    wr.writerow(['domain_name'])
    for row in domain_name:
        wr.writerow([row])
    f.close()

    return render_template("adminportal_countTable.html", countTable = countTable)


@app.route("/privateBook_and_countPerDomain", methods=['POST'])
def privateBook_and_countPerDomain():
    session['semail'] = semail
    session['passwd'] = passwd
    
    email = request.form.get('email')
    filename2 = f'{email}.csv'
    file_path2 = '{}'.format(filename2)
    read_file = open(file_path2, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    privateCsv = []
    for row in reader:
        privateCsv.append(row) #개인주소록을 화면에 표시하기 위한 코드
    read_file.close()

    email = request.form.get('email')
    filename2 = f'{email}.csv'
    file_path2 = '{}'.format(filename2)
    read_file = open(file_path2, encoding='utf-8')
    reader = csv.reader(read_file, delimiter=',')
    privateCsv_domain = []
    for row in reader:
        privateCsv_domain.append(row[2]) #도메인 별 빈도표를 화면에 표시하기 위한 코드
    read_file.close()

    domain_name = []

    for row in privateCsv_domain:
            email_String = row
            email_Divided = email_String.split('@') #split의 인자 기준으로 나누고 리스트를 만들어줌
            domain_name.append(email_Divided[1]) #뒤에 있는 domain_name만 사용 
    
    countTable = []
    result = Counter(domain_name)
    for key in result:
        countTable.append( [key,result[key]] )
    
    return render_template("privatePortal_countTable.html", privateCsv = privateCsv , email=email , countTable = countTable)

'''
@app.route("/<sid>/credits")
def credits(sid):
    conn = pg.connect(connect_string)
    cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor) #실수주의! DictCursor 옵션을 주어야 ['sid'] 등 field명으로 접근 가능

    sql = f"SELECT cl.name, cl.course_id, cl.year_open as year, cl.credit, cr.grade FROM class cl, credits cr WHERE cr.sid='{sid}' AND cl.class_id=cr.class_id"
    cur.execute(sql)
    rows = cur.fetchall()

    for row in rows:
        print(row)

    conn.close()
    return render_template("credits.html", credits = rows, sid=sid) #이건 다 전달해야 됨
'''

def get_csv(filename):
    file_path = '{}'.format(filename)
    #print(file_path)

    try:
        read_file = open(file_path, encoding='utf-8')
        reader = csv.reader(read_file, delimiter=',')
        #print(reader)
        result = []
        for row in reader:
            #print(row)
            result.append(row)
    except:
        print("csv error")
        return -1

    read_file.close()
    return result

def get_new_csv(csv1,csv2):
    file_path = 'addressBook.csv'

    try:
        f = open('addressBook.csv', 'r', encoding='utf-8') #아직 주소록이 생성되지 않았다면
        f.close()
    except:
        try:
            f = open(file_path, 'w', encoding='utf-8', newline='')
            wr = csv.writer(f)
            for row_contacts in csv1:
                for row_students in csv2:
                    if(row_students[0] == row_contacts[0]):
                        wr.writerow([row_students[2], row_students[0], row_contacts[1], row_contacts[2]])
                        break
        except:
            print("create new_csv error")
            return -1
        f.close()

    try:
        read_file = open(file_path, encoding='utf-8')
        reader = csv.reader(read_file, delimiter=',')
        #print(reader)
        result = []
        for row in reader:
            #print(row)
            result.append(row)
    except:
        print("read new_csv error")
        return -1   
    read_file.close()
    return result

if __name__ == "__main__":
    app.run(debug=True)

