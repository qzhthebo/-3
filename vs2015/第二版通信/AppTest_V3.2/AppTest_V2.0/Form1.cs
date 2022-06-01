using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Communication;
using Dongzr.MidiLite;

namespace AppTest_V2._0
{
    public partial class Form1 : Form
    {
        MmTimer mmtimer;//定义多媒体定时器
        byte header = 0xF0;
        byte tail = 0x0F;

        public SerialPort com = new SerialPort();

        Form2 f2 = new Form2();


        int flag_ser=0;
        public Form1()
        {
            InitializeComponent();

  


  
            mmtimer = new MmTimer();
            mmtimer.Mode = MmTimerMode.Periodic;//，欧式为周期运行
            mmtimer.Interval = 1;//周期1ms
            mmtimer.Tick += new EventHandler(mmtick);//当周期到了之后，使用函数
            mmtimer.Start();

        }

        private void mmtick(object sender, EventArgs e)
        {
            
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            MBmain.SetImageList(ILmain);//加入图片到组件中
            MBremo.SetImageList(ILremo);
            MBpara.SetImageList(ILpara);
            MBimu.SetImageList(ILimu);
            MBgps.SetImageList(ILgps);

            RBser.SetImageList(ILser);
        }



        private void MBmain_Load(object sender, EventArgs e)
        {

        }
        private void MBmain_Click(object sender, EventArgs e)//主页面按钮按下
        {
   
           

        }

        private void MBremo_Load(object sender, EventArgs e)
        {

        }
        private void MBremo_Click(object sender, EventArgs e)//遥控器界面按钮按下
        {
   
        }

        private void MBpara_Load(object sender, EventArgs e)
        {

        }
        private void MBpara_Click(object sender, EventArgs e)//控制参数按钮按下
        {

        }

        private void MBimu_Load(object sender, EventArgs e)
        {

        }
        private void MBimu_Click(object sender, EventArgs e)//IMU按钮按下
        {

        }

        private void MBgps_Load(object sender, EventArgs e)
        {

        }
        private void MBgps_Click(object sender, EventArgs e)//GPS按钮按下
        {

        }

        private void RBser_Load(object sender, EventArgs e)//
        {
            // 
        }
        private void RBser_Click(object sender, EventArgs e)//打开/关闭按钮按下
        {
            if (flag_ser != 0)
            {
                
                RBser.SecondState(false);
                flag_ser = 1- flag_ser;
                com.Close();
            }
            else
            {
                
                RBser.SecondState(true);
                flag_ser = 1- flag_ser;
                com.Open(cbComNum.Text, int.Parse(tbBaudRate.Text));//打开串口
            }

        }

        private void btreno_Click(object sender, EventArgs e)//刷新按钮按下
        {
            btreno.BackgroundImage = Properties.Resources.reno_down;//初始化图片

            cbComNum.Items.Clear();
            foreach (string str in com.AvalibleSerialPort())
            {
                cbComNum.Items.Add(str);
            }
            if (cbComNum.Items.Count >= 1)
                cbComNum.Text = cbComNum.Items[0].ToString();
        }
        private void btreno_MouseLeave(object sender, EventArgs e)//鼠标移开
        {
            btreno.BackgroundImage = Properties.Resources.reno_no;
        }
        private void btreno_MouseHover(object sender, EventArgs e)//鼠标划过
        {
            btreno.BackgroundImage = Properties.Resources.reno_pass;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            int n = 0;
   
        }

        private void button1_Click(object sender, EventArgs e)//强制开关
        {
            f2.Show();


        }

        private void timer2_Tick(object sender, EventArgs e)//边缘数据
        {
            int n = 1;
 
            if (n == 1)
            {
                pictureBox6.BackgroundImage = Properties.Resources.已连接;
            }
            else
            {
                pictureBox6.BackgroundImage = Properties.Resources.未连接;
            }
            n = Convert.ToInt16(2);
            switch (n)//电量
            {
                case 0:
                    pictureBox5.BackgroundImage = Properties.Resources.l0;
                    break;
                case 1:
                    pictureBox5.BackgroundImage = Properties.Resources.l1;
                    break;
                case 2:
                    pictureBox5.BackgroundImage = Properties.Resources.l2;
                    break;
                case 3:
                    pictureBox5.BackgroundImage = Properties.Resources.l3;
                    break;
                case 4:
                    pictureBox5.BackgroundImage = Properties.Resources.l4;
                    break;
            }
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {

        }
    }
}
