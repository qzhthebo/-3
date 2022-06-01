using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Communication;

namespace CommunicationDemo
{
    public partial class Form1 : Form
    {
        UIDLink uidLink;
        SerialPort com;
        DataFrame txData = new DataFrame();
        DataFrame rxData = new DataFrame();

        //FunctionCode功能码对象
        //                                                     name                     code       txLen    rxLen

        FunctionCode IMU_DATA  = new FunctionCode(            "IMUData",                0x01,      1,        1);    
        FunctionCode HIGH_FREQUENCY_DATA = new FunctionCode(  "HighFrequencyData",      0x02,      42,       0);                      
        FunctionCode LOW_FREQUENCY_DATA = new FunctionCode(   "LowFrequencyData",       0x03,      12,       0);

        //声明并初始化功能码列表对象
        List<FunctionCode> fnList = new List<FunctionCode>();

        public Form1()
        {
            InitializeComponent();

            com = new SerialPort();

            //将上面定义的功能码对象逐个添加到功能码列表中
            fnList.Add(IMU_DATA);
            fnList.Add(HIGH_FREQUENCY_DATA);
            fnList.Add(LOW_FREQUENCY_DATA);

            //初始化UIDLink对象
            uidLink = new UIDLink(com, fnList, 0xAA, 0xAF);
        }
        //串口刷新按钮函数
        private void freshBt_Click(object sender, EventArgs e)
        {
            comBox.Items.Clear();                                 //清空下拉串口列表
            foreach (string str in com.AvalibleSerialPort())      //判断是否有可获取串口
                comBox.Items.Add(str);                            //对可获取串口都填充到下拉列表
            if (comBox.Items.Count > 0)
                comBox.SelectedIndex = 0;                         //默认情况下拉列表选中第一个
        }
        //绑定串口函数
        private void openBt_Click(object sender, EventArgs e)
        {
            if (!com.IsAvalible())                                //判断是否有可获取串口
            {
                if (comBox.SelectedIndex < 0) return;
                if (com.Open(comBox.SelectedItem.ToString(), 115200))//绑定下拉列表中选中项的串口，同时赋波特率为115200
                {
                    comBox.Enabled = false;                         //绑定之后下拉列表不可点击
                    com.ClearRxBuffer();
                    openBt.Text = "关闭串口";
                }
            }
            else                                                    //关闭串口
            {
                com.Close();
                comBox.Enabled = true;
                openBt.Text = "打开串口";
            }
        }

        private void sendBt_Click(object sender, EventArgs e)
        {
            String txtData = text.Text.ToString();          //获取文本框中的文本数据
            int sendData = Convert.ToInt32(txtData);        //将字符串数值转换为整型数值

            txData.fnCode = IMU_DATA;                       //填充功能码
            txData.FillData(0, sendData);                   //填充数据
            uidLink.SendFrame(txData);                      //发送数据帧
        }
        //定时器检测接收到的数据
        private void getData_Tick(object sender, EventArgs e)
        {
            if (uidLink.CheckFrame(ref rxData))                        //判断接收到的数据帧是否有效
            {
                String txtHeader = rxData.header.ToString();           //获取帧头
                String txtCode = rxData.fnCode.Code.ToString();        //获取功能码
                String txtDateLength = rxData.dataLength.ToString();   //获取数据长度

                if (rxData.fnCode == IMU_DATA)                         //判断数据帧的功能码
                {
                    byte data = 0;
                    rxData.GetData(0, ref data);                       //获取数据位数据
                    getBox.Text = data.ToString();                     //把接收到的数据显示出来
                }
            }
        }
    }
}
