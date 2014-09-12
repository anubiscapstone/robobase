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
        int BaseVal = 1500;
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
            sport.Write("#0 P1500 #1 P1500 #2 P1500 #3 P1500 #4 P1500 #5 P1500 #6 P1500 #7 P1500 #8 P1500 #9 P1500 #10 P1500 #11 P1500 #12 P1500 #13 P1500 #14 P1500 #15 P1500\r");
            
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            sport.Write("#0L #1L #2L #3L #4L #5L #6L #7L #8L #9L #10L #11L #12L #13L  #14 P1500 #15 P1500\r");
            sport.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            double angle = (double)numericUpDown1.Value;
            int pulse = (int)(angle * (10)) + 600; 
            sport.Write("#0 P" + pulse +"\r");
            
        }

        private void MainForm_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Q:
                    if (!(BaseVal <= 500))
                        BaseVal -= 20;
                    sport.Write("#0 P" + BaseVal + "\r");
                    break;
      
                case Keys.E:
                    if (!(BaseVal >= 2500))
                        BaseVal += 20;
                    sport.Write("#0 P" + BaseVal + "\r");
                    break;

                case Keys.S:
                    sport.Write("#15 P1600 #14 P1400\r");
                    break;

                case Keys.W:
                    sport.Write("#15 P1400 #14 P1600\r");
                    break;

                case Keys.A:
                    sport.Write("#15 P1400 #14 P1400\r");
                    break;

                case Keys.D:
                    sport.Write("#15 P1600 #14 P1600\r");
                    break;
            
                case Keys.K:
                    sport.Write("#0L #1L #2L #3L #4L #5L #6L #7L #8L #9L #10L #11L #12L #13L #14 P1500 #15 P1500\r");
                    break;

                case Keys.L:
                    sport.Write("#0 P1500 #1 P1500 #2 P1500 #3 P1500 #4 P1500 #5 P1500 #6 P1500 #7 P1500 #8 P1500 #9 P1500 #10 P1500 #11 P1500 #12 P1500 #13 P1500 #14 P1500 #15 P1500\r");
                    break;

            }
            
        }

    }
}
