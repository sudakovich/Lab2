import threading
from dataclasses import dataclass
import socket, struct, time

M_ERROR		= 0
M_INIT		= 1
M_EXIT		= 2
M_GETDATA	= 3
M_NODATA	= 4
M_DATA		= 5
M_CONFIRM	= 6

M_BROKER	= 0
M_ALL		= 10
M_USER		= 100


@dataclass
class MsgHeader:
	To: int = 0
	From: int = 0
	Type: int = 0
	Size: int = 0

	def Send(self, s):
		s.send(struct.pack(f'iiii', self.To, self.From, self.Type, self.Size))

	def Receive(self, s):
		try:
			(self.To, self.From, self.Type, self.Size) = struct.unpack('iiii', s.recv(16))
		except:
			self.Size = 0
			self.Type = M_NODATA

class Message:
	ClientID = 0

	def __init__(self, To = 0, From = 0, Type = M_DATA, Data=""):
		self.Header = MsgHeader(To, From, Type, len(Data))
		self.Data = Data

	def Send(self, s):
		self.Header.Send(s)
		if self.Header.Size > 0:
			s.send(struct.pack(f'{self.Header.Size}s', self.Data.encode('cp866')))

	def Receive(self, s):
		self.Header.Receive(s)
		if self.Header.Size > 0:
			self.Data = struct.unpack(f'{self.Header.Size}s', s.recv(self.Header.Size))[0].decode('cp866')

	def SendMessage(To, Type = M_DATA, Data=""):
		HOST = '127.0.0.1'
		PORT = 54321
		with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
			s.connect((HOST, PORT))
			m = Message(To, Message.ClientID, Type, Data)
			m.Send(s)
			m.Receive(s)
			s.close()
			if m.Header.Type == M_CONFIRM:
				Message.ClientID = m.Header.To
				#print(Message.ClientID)
			return m