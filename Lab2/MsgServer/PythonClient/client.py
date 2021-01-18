import threading
from msg import *


def ProcessMessages():
	while True:
		m = Message.SendMessage(M_BROKER, M_GETDATA)
		if m.Header.Type == M_DATA:
			print(m.Header.From)
			print(m.Data)
		else:
			time.sleep(1)

 
def Client():
	Message.SendMessage(M_BROKER, M_INIT)
	t = threading.Thread(target=ProcessMessages)
	t.start()
	while True:
		ID=int(input("Enter ID(10 - all)\n"))
		print("Text = ")
		Message.SendMessage(ID, M_DATA, input())

Client()
