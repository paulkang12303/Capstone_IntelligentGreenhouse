import wx
import serial
import requests

class MainFrame(wx.Frame):
	def __init__(self):
		wx.Frame.__init__(self, parent=None, id=1)
		self.SetTitle("GreenHouse")
		self.SetSize((320,460))
		self.Center()
		
		self.color_red = wx.Colour(255, 0, 0)
		self.color_green = wx.Colour(0, 255, 0)
		self.color_blue = wx.Colour(0, 0, 255)
		self.color_yellow = wx.Colour(225, 225, 0)
		
		font_heading= wx.Font(10, wx.ROMAN, wx.NORMAL, wx.BOLD)
		heading_1 = wx.StaticText(self,-1,"Environment Parameters: ", (20,10))
		heading_1.SetFont(font_heading)
		
		wx.StaticLine(self, pos=(20, 30), size=(280,1))
		
		wx.StaticText(self,-1,"Temperature: ", (40,40))
		self.st_T = wx.StaticText(self, -1, " ", (200, 40))
		self.st_TU = wx.StaticText(self, -1, " Celsius ", (230, 40))
		wx.StaticText(self,-1,"Humidity: ", (40,60))
		self.st_H = wx.StaticText(self, -1, " ", (200, 60))
		self.st_HU = wx.StaticText(self, -1, " Persent ", (230, 60))
		wx.StaticText(self,-1,"CO2: ", (40,80))
		self.st_C = wx.StaticText(self, -1, " ", (200, 80))
		self.st_CU = wx.StaticText(self, -1, " PPM ", (230, 80))
		wx.StaticText(self,-1,"Illumination: ", (40,100))
		self.st_I = wx.StaticText(self, -1, " ", (200, 100))
		self.st_IU = wx.StaticText(self, -1, " xl ", (230, 100))
		wx.StaticText(self,-1,"Soil Moisture: ", (40,120))
		self.st_S = wx.StaticText(self, -1, " ", (200, 120))
		self.st_SU = wx.StaticText(self, -1, " Persent ", (230, 120))
		
		self.status_T = wx.Panel(self, pos=(20, 42), size=(10, 10))
		self.status_H = wx.Panel(self, pos=(20, 62), size=(10, 10))
		self.status_C = wx.Panel(self, pos=(20, 82), size=(10, 10))
		self.status_I = wx.Panel(self, pos=(20, 102), size=(10, 10))
		self.status_S = wx.Panel(self, pos=(20, 122), size=(10, 10))
		
		heading_2 = wx.StaticText(self,-1,"Device Status: ", (20,160))
		heading_2.SetFont(font_heading)
		wx.StaticLine(self, pos=(20, 180), size=(280,1))
		
		self.lam = wx.Panel(self, pos=(30, 192), size=(10, 10))
		self.lam.SetBackgroundColour(self.color_yellow)
		wx.StaticText(self,-1,"Daylight Lamp", (50,190))
		
		self.shu = wx.Panel(self, pos=(160, 192), size=(10, 10))
		self.shu.SetBackgroundColour(self.color_yellow)
		wx.StaticText(self,-1,"Shutter", (180,190))		

		self.coo = wx.Panel(self, pos=(30, 212), size=(10, 10))
		self.coo.SetBackgroundColour(self.color_yellow)
		wx.StaticText(self,-1,"AC Cooler", (50,210))
		
		self.hea = wx.Panel(self, pos=(160, 212), size=(10, 10))
		self.hea.SetBackgroundColour(self.color_yellow)
		wx.StaticText(self,-1,"AC Heater", (180,210))	
		
		self.co2 = wx.Panel(self, pos=(30, 232), size=(10, 10))
		self.co2.SetBackgroundColour(self.color_yellow)
		wx.StaticText(self,-1,"CO2 Generator", (50,230))
		
		self.ven = wx.Panel(self, pos=(160, 232), size=(10, 10))
		self.ven.SetBackgroundColour(self.color_yellow)
		wx.StaticText(self,-1,"Ventilator", (180,230))	
		
		self.wat = wx.Panel(self, pos=(30, 252), size=(10, 10))
		self.wat.SetBackgroundColour(self.color_yellow)
		wx.StaticText(self,-1,"Water Sprayer", (50,250))
		
		self.ala = wx.Panel(self, pos=(160, 252), size=(10, 10))
		self.ala.SetBackgroundColour(self.color_yellow)
		wx.StaticText(self,-1,"Alarm", (180,250))
		
		self.btn_start = wx.Button(self, -1, "START", (200, 280), (70, 20))
		self.Bind(wx.EVT_BUTTON, self.onButtonClick, self.btn_start)
	
		heading_3 = wx.StaticText(self,-1,"Configuration: ", (20,300))
		heading_3.SetFont(font_heading)
		wx.StaticLine(self, pos=(20, 320), size=(280,1))
	
		wx.StaticText(self,-1,"Parameter: ", (20,330))
		cb1_str = ['Temperature','Illumination','CO2','Moisture']
		self.cb1 = wx.ComboBox(self, pos = (20, 350), choices = cb1_str, style = wx.CB_READONLY)
		self.Bind(wx.EVT_COMBOBOX, self.OnSelect, self.cb1)
		
		wx.StaticText(self,-1,"Upper Limit: ", (150,330))
		self.tc1 = wx.TextCtrl(self,-1," ",pos=(150,350),size=(60,22))
	
		wx.StaticText(self,-1,"Lower Limit: ", (220,330))
		self.tc2 = wx.TextCtrl(self,-1," ",pos=(220,350),size=(60,22))
	
		self.btn_config = wx.Button(self, -1, "CONFIGURATE", (150, 385), (135, 20))
		self.Bind(wx.EVT_BUTTON, self.onButtonClick, self.btn_config)
	
		self.timer = wx.Timer(self)
		self.Bind(wx.EVT_TIMER, self.timerInterval, self.timer)
		
	def onButtonClick(self, event):
		if event.GetEventObject() == self.btn_start:
			val = self.btn_start.GetLabelText()
			if (val == 'START'):
				self.ser = serial.Serial('com5',115200,timeout=2)
				self.timer.Start(5000)
				self.btn_start.SetLabelText('STOP')
			if (val == 'STOP'):
				self.ser.close()
				self.timer.Stop()
				self.btn_start.SetLabelText('START')
		if event.GetEventObject() == self.btn_config:
			val_P = self.cb1.GetStringSelection()
			val_Upper = self.tc1.GetValue()
			val_Lower = self.tc2.GetValue()
			info_str = "{}'s upper limit has been set to {}. \n Lower limit has been set to {}".format(val_P,val_Upper,val_Lower)
			dlgmsg = wx.MessageDialog(self, info_str,'setup info',wx.OK )
			dlgmsg.Center()
			dlgmsg.ShowModal()
			dlgmsg.Destroy()
			self.ser = serial.Serial('com5',115200,timeout=2)
			self.ser.write("{};{};{}\r\n".format(val_P,val_Upper,val_Lower))
			self.ser.close()
	
	def OnSelect(self, event):
		if (self.cb1.GetStringSelection()=='Temperature'):
			self.tc1.SetValue('30.0')
			self.tc2.SetValue('18.0')
		if (self.cb1.GetStringSelection()=='Illumination'):
			self.tc1.SetValue('20000')
			self.tc2.SetValue('10')
		if (self.cb1.GetStringSelection()=='CO2'):
			self.tc1.SetValue('2000')
			self.tc2.SetValue('500')
		if (self.cb1.GetStringSelection()=='Moisture'):
			self.tc1.SetValue('90.0')
			self.tc2.SetValue('30.0')
		
	def timerInterval(self, event):
		RX_string = self.ser.readline()
		#print(RX_string)
		I = int(str(RX_string)[10:15])
		self.st_I.SetLabel(str(I))
		T = int(str(RX_string)[25:30])/100.0
		self.st_T.SetLabel(str(T))
		H = int(str(RX_string)[33:38])/100.0
		self.st_H.SetLabel(str(H))		
		C = int(str(RX_string)[51:56])
		self.st_C.SetLabel(str(C))	
		S = int(str(RX_string)[62:67])/100.0
		self.st_S.SetLabel(str(S))
		
		if (str(RX_string)[68] == '1'):
			self.status_T.SetBackgroundColour(self.color_blue)
			self.status_T.Refresh()
		else:
			self.status_T.SetBackgroundColour(self.color_red)
			self.status_T.Refresh()
			
		if (str(RX_string)[69] == '1'):
			self.status_H.SetBackgroundColour(self.color_blue)
			self.status_H.Refresh()
		else:
			self.status_H.SetBackgroundColour(self.color_red)
			self.status_H.Refresh()
					
		if (str(RX_string)[71] == '1'):
			self.status_C.SetBackgroundColour(self.color_blue)
			self.status_C.Refresh()
		else:
			self.status_C.SetBackgroundColour(self.color_red)
			self.status_C.Refresh()
					
		if (str(RX_string)[70] == '1'):
			self.status_I.SetBackgroundColour(self.color_blue)
			self.status_I.Refresh()
		else:
			self.status_I.SetBackgroundColour(self.color_red)
			self.status_I.Refresh()
					
		if (str(RX_string)[72] == '1'):
			self.status_S.SetBackgroundColour(self.color_blue)
			self.status_S.Refresh()
		else:
			self.status_S.SetBackgroundColour(self.color_red)
			self.status_S.Refresh()		
		s_T = str(RX_string)[68];
		s_H = str(RX_string)[69];
		s_I = str(RX_string)[70];
		s_C = str(RX_string)[71];
		s_S = str(RX_string)[72];
		
		s_lam = str(RX_string)[74];
		s_sht = str(RX_string)[75];
		s_htr = str(RX_string)[76];
		s_clr = str(RX_string)[77];
		s_co2 = str(RX_string)[78];
		s_ven = str(RX_string)[79];
		s_spr = str(RX_string)[80];
		s_alm = str(RX_string)[81];
		
		if (s_lam == "1"):
			self.lam.SetBackgroundColour(self.color_green)
			self.lam.Refresh()
		else:
			self.lam.SetBackgroundColour(self.color_yellow)
			self.lam.Refresh()
		
		if (s_sht == "1"):
			self.shu.SetBackgroundColour(self.color_green)
			self.shu.Refresh()
		else:
			self.shu.SetBackgroundColour(self.color_yellow)
			self.shu.Refresh()
			
		if (s_htr == "1"):
			self.hea.SetBackgroundColour(self.color_green)
			self.hea.Refresh()
		else:
			self.hea.SetBackgroundColour(self.color_yellow)
			self.hea.Refresh()
			
		if (s_clr == "1"):
			self.coo.SetBackgroundColour(self.color_green)
			self.coo.Refresh()
		else:
			self.coo.SetBackgroundColour(self.color_yellow)
			self.coo.Refresh()		
			
		if (s_co2 == "1"):
			self.co2.SetBackgroundColour(self.color_green)
			self.co2.Refresh()
		else:
			self.co2.SetBackgroundColour(self.color_yellow)
			self.co2.Refresh()	
			
		if (s_ven == "1"):
			self.ven.SetBackgroundColour(self.color_green)
			self.ven.Refresh()
		else:
			self.ven.SetBackgroundColour(self.color_yellow)
			self.ven.Refresh()	

		if (s_spr == "1"):
			self.wat.SetBackgroundColour(self.color_green)
			self.wat.Refresh()
		else:
			self.wat.SetBackgroundColour(self.color_yellow)
			self.wat.Refresh()	
			
		if (s_alm == "1"):
			self.ala.SetBackgroundColour(self.color_green)
			self.ala.Refresh()
		else:
			self.ala.SetBackgroundColour(self.color_yellow)
			self.ala.Refresh()	
		
		status = "1" + s_lam + s_sht + s_htr + s_clr + s_co2 + s_ven + s_spr + s_alm + s_T + s_H + s_I + s_C + s_S + "0";
		#status = "100000000111110"
		TX_string = "http://project.hmddesign.ca/add.php?tem={}&Hum={}&Ill={}&CO2={}&soi={}&Sta={}".format(str(T),str(H),str(I),str(C),str(S),status)
		requests.put(TX_string)

class MyApp(wx.App):
	def OnInit(self):
		self.frame = MainFrame()
		self.frame.Show()
		return True

if __name__ == '__main__':
	app = MyApp()
	app.MainLoop()
		
		
		
