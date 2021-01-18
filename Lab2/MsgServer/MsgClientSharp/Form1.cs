using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;

namespace MsgClientSharp
{
    public partial class Form1 : Form
    {
        [DllImport("MsgDLL.dll")]
        private static extern uint OnLaunch();

        [DllImport("MsgDLL.dll")]
        private static extern void Send(uint m_User, uint m_ID, uint m_Code, string m_Text);

        [DllImport("MsgDLL.dll")]
        private static extern int Get(uint m_ID);

        [DllImport("MsgDLL.dll")]
        private static extern uint GetIDMessage();

        [DllImport("MsgDLL.dll")]
        private static extern uint GetSizeMessage();

        [DllImport("MsgDLL.dll")]
        private static extern char GetTextMessage(uint i);
        uint m_ID;

        private delegate void InvokeDelegate();

        uint M_DATA = 5;
        uint M_EXIT = 3;
        private void CheckForMessages()
        {
            while (true)
            {
                Thread.Sleep(1000);
                if (Get(m_ID) == 5)
                {
                    BeginInvoke(new InvokeDelegate(ChangeListBox)); 
                }
            }
        }
        public Form1()
        {
            InitializeComponent();
            m_ID = OnLaunch();
            listBox1.Items.Add(m_ID);
            Thread myThread = new Thread(CheckForMessages);
            myThread.Start();
        }
        void ChangeListBox()
        {
            uint size = GetSizeMessage();
            string Text = "";
            for (uint i = 0; i < size; i++)
            {
                Text += GetTextMessage(i);
            }
            listBox1.Items.Add(GetIDMessage() + " " + Text);
        }
        private void SendButton_Click(object sender, EventArgs e)
        {
            Send(uint.Parse(IdTextBox.Text), m_ID, M_DATA, TextTextBox.Text);
        }

        private void Form1_FormClosed(Object sender, FormClosedEventArgs e)
        {
            Send(0, m_ID, M_EXIT, "");
        }
    }
}
