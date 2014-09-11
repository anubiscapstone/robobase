using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace SerialPortDotNET
{
    public partial class Form1 : Form
    {
        public SerialPort sport;
        public Form1()
        {
            InitializeComponent();
            sport = new SerialPort("COM1");
            sport.BaudRate = 115200;
            sport.Parity = System.IO.Ports.Parity.None;
            sport.DataBits = 8;
            sport.StopBits = System.IO.Ports.StopBits.One;
            sport.Open();
            sport.Write("#1 P1500 #2 P1500 #3 P1500 #4 P1500 #5 P1500 #6 P1500 #7 P1500 #8 P1500 #9 P1500 #10 P1500 #11 P1500 #12 P1500 #13 P1500 #14 P1500 #15 P1500\r");
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            sport.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            sport.Write("#0 P" + numericUpDown1.Value +"\r");
            
        }
    }
}
