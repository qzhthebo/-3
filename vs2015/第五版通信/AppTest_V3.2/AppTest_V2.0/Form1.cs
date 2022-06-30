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
        public SerialPort com = new SerialPort();//实例化对象
        public UIDLink uidLink;
        public List<FunctionCode> fnList = new List<FunctionCode>(); //功能码的队列
        //                                           名字        功能码  发送长度   接收长度（上位机）
        FunctionCode COM_PID_PARAMS = new FunctionCode("COM_PID_PARAMS", 0x01, 36, 36);//单片机发送就是上位机的接收
        FunctionCode COM_FLY_DATA = new FunctionCode("COM_FLY_DATA", 0x02, 1, 53);
        FunctionCode bagt = new FunctionCode("bagt", 0x03, 1,1);
        public int flag_ser = 0;//打开/关闭串口按钮使用
        //******串口******
        //******多窗口******
        public Gps winGps;//GPS
        public Imu winImu;//IMU
        public MainPage winMainPage;//主页面
        public RemoteControl winRemoteControl;//遥控器
        public ControlParameters winControlParameters;//控制参数

        DataFrame reFrame;
        DataFrame txData = new DataFrame();

        float[] x = new float[9];//通道
        long xx1 = 0;
        float[] y0 = new float[19];//传PID
        float[] y1 = new float[19];//传PID
        long yy0 = 0;
        float[] y = new float[18];//收PID
        long yy1 = 0;
        float[] z = new float[12];//IMU收
        long zz1 = 0;
        float[] d = new float[16];
        long dd1 = 0;
        //各部分参数

        double t = 0;
        double t1 = 0;
        double t2 = 0;
        double t3 = 0;
        double sum = 0;
        int num = 0;//帧率计算

        UInt16 seq = 0;//包号
        byte flystate = 0;//飞机状态

        float eltctric=0;
        float statu = 1;
        float air=0;//状态栏

        bool bldate=false;

        int nt=0;//接收次数

        int qiangzhi = 0;//强制关闭开关

        byte isor = 12;
        public Form1()
        {
            InitializeComponent();

            //********************************************
            fnList.Add(COM_PID_PARAMS);
            fnList.Add(COM_FLY_DATA);
            fnList.Add(bagt);
            uidLink = new UIDLink(com, fnList, 0xAA, 0xAF);
            //********************************************

            foreach (string str in com.AvalibleSerialPort())//返回所有串口返回所有的串口值,但是返回的是字符串数组
            {
                cbComNum.Items.Add(str);//将返回的数组加到下拉框中
            }

            winGps = new Gps();
            winImu = new Imu();
            winMainPage = new MainPage();
            winRemoteControl = new RemoteControl();
            winControlParameters = new ControlParameters();

            reFrame = new DataFrame();

            mmtimer = new MmTimer();
            mmtimer.Mode = MmTimerMode.Periodic;//，欧式为周期运行
            mmtimer.Interval = 1;//周期1ms
            mmtimer.Tick += new EventHandler(mmtick);//当周期到了之后，使用函数
            mmtimer.Start();

        }

        private void mmtick(object sender, EventArgs e)
        {
            if (flag_ser == 1)
            {
                t1 = t2;
                while (uidLink.CheckFrame(ref reFrame) == CheckFrameResult.CHECK_SUCCESS)
                {
                    bldate = true;

                    if (num < 11)
                    {
                        num++;
                        t2 = DateTime.Now.Ticks;
                        t3 = ((t2 - t1) / 10000);
                        sum += t3;
                    }
                    else
                    {
                        t2 = DateTime.Now.Ticks;
                        num = 0;
                        t = 10 / sum;
                        sum = 0;
                    }
                    int i = 0;
                    Int16 PID = 0;
                    Int16 fly = 0;
                    switch (reFrame.fnCode.Code)
                    {
                        case 1:
                            yy1++;
                            for (i = 0; i < 18; i++)
                            {
                                reFrame.GetData(i * 2, ref PID);
                                y[i]=Convert.ToSingle(PID) / 1000;
                            }
                            break;
                        case 2:
                            zz1++;
                            xx1++;
                            reFrame.GetData(0, ref seq);//包号
                            for (i = 1; i < 4; i++)//横滚俯仰
                            {
                                reFrame.GetData(i * 2, ref fly);
                                z[i-1]= Convert.ToSingle(fly) / 100;
                            }
                            for(i=4;i<13;i++)//加速度，角速度，陀螺仪
                            {
                                reFrame.GetData(i * 2, ref fly);
                                z[i - 1] = Convert.ToSingle(fly);
                            }
                            reFrame.GetData(26, ref air);//气压
                            for(i=14;i<17;i++)//光流，激光
                            {
                                reFrame.GetData(i * 2, ref fly);
                            }
                            for(i=17;i<26;i++)//通道
                            {
                                reFrame.GetData(i * 2, ref fly);
                                x[i - 17] = Convert.ToSingle(fly);
                            }
                            reFrame.GetData(52, ref flystate);//飞行状态
                            break;
                        case 3:
                            reFrame.GetData(0, ref isor);
                            break;
                    }
                }
            }
            else
            {
                t1 = 0;
            }
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
            MBremo.Normal();
            MBpara.Normal();
            MBimu.Normal();
            MBgps.Normal();
            winMainPage.Height = groupBox1.Height;
            winMainPage.Width = groupBox1.Width;
            winMainPage.Show();//显示窗口——主页面
            groupBox1.Controls.Clear();//清除之前的窗口
            groupBox1.Controls.Add(winMainPage);

        }

        private void MBremo_Load(object sender, EventArgs e)
        {

        }
        private void MBremo_Click(object sender, EventArgs e)//遥控器按钮按下
        {
            MBmain.Normal();
            MBpara.Normal();
            MBimu.Normal();
            MBgps.Normal();
            winRemoteControl.Show();//显示窗口——遥控器
            groupBox1.Controls.Clear();//清除之前的串窗口
            groupBox1.Controls.Add(winRemoteControl);
        }

        private void MBpara_Load(object sender, EventArgs e)
        {

        }
        private void MBpara_Click(object sender, EventArgs e)//控制参数按钮按下
        {
            MBmain.Normal();
            MBremo.Normal();
            MBimu.Normal();
            MBgps.Normal();
            winControlParameters.Show();//显示窗口——控制参数
            groupBox1.Controls.Clear();//清除之前的窗口
            groupBox1.Controls.Add(winControlParameters);
        }

        private void MBimu_Load(object sender, EventArgs e)
        {

        }
        private void MBimu_Click(object sender, EventArgs e)//IMU按钮按下
        {
            MBmain.Normal();
            MBremo.Normal();
            MBpara.Normal();
            MBgps.Normal();

            winImu.Height = groupBox1.Height;
            winImu.Width = groupBox1.Width;
            winImu.Show();//显示窗口——IMU

            groupBox1.Controls.Clear();//清除之前的窗口
            groupBox1.Controls.Add(winImu);
        }

        private void MBgps_Load(object sender, EventArgs e)
        {

        }
        private void MBgps_Click(object sender, EventArgs e)//GPS按钮按下
        {
            MBmain.Normal();
            MBremo.Normal();
            MBpara.Normal();
            MBimu.Normal();

            winGps.Show();//显示窗口——GPS
            groupBox1.Controls.Clear();//清除之前的窗口
            groupBox1.Controls.Add(winGps);
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
                flag_ser = 0;
                statu = 0;
                com.Close();
            }
            else
            {
                
                RBser.SecondState(true);
                flag_ser = 1;
                statu = 1;
                com.Open(cbComNum.Text, int.Parse(tbBaudRate.Text));
            }

        }

        private void btreno_Click(object sender, EventArgs e)//刷新按钮按下
        {
            btreno.BackgroundImage = Properties.Resources.reno_down;

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
            winImu.chuanzhi(z);//imu
            if (flag_ser == 1)
            {
               
                    if (zz1 > 0)
                    {
                        winImu.chuanzhi(z);//imu
                       y1=winMainPage.chuanzhi(z,y);//包号
                    zz1--;
                    }
                if (bldate == true)
                {

                    //winMainPage.chuanzhi(seq);//包号

                winRemoteControl.chuanzhi(x);//通道

                    //winGps.chuanzhi(z);//暂无
                    if (y1[18] == 1)//证明改变数据了
                    {
                        txData.fnCode = COM_PID_PARAMS;
                        Int16 xx = 0;
                        for (int i = 0; i < 18; i++)
                        {
                            xx = Convert.ToInt16(y1[i] * 1000);
                            txData.FillData(i * 2, xx);

                        }

                        uidLink.SendFrame(txData);
                    }

                    y0 = winControlParameters.chuanzhi(y);

                    if (y0[18] == 1)//证明改变数据了
                    {
                        txData.fnCode = COM_PID_PARAMS;
                        Int16 xx = 0;
                        for (int i = 0; i < 18; i++)
                        {
                            xx = Convert.ToInt16(y0[i] * 1000);
                            txData.FillData(i * 2, xx);

                        }
                        
                        uidLink.SendFrame(txData);
                    }
                }
                //txData.fnCode = bagt;
                //txData.FillData(0, isor);
                //uidLink.SendFrame(txData);

            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            qiangzhi = 1-qiangzhi;
            txData.fnCode = COM_FLY_DATA;
            byte xx = 1;
             txData.FillData(0, xx);
            uidLink.SendFrame(txData);

        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            int n = 0;
            if(qiangzhi==1)
            {
                txData.fnCode = COM_FLY_DATA;
                byte xx = 1;
                txData.FillData(0, xx);
                uidLink.SendFrame(txData);
                textBox2.Text = isor.ToString();//气压
            }
            //textBox2.Text = air.ToString();//气压

            textBox1.Text = t.ToString();
            n = Convert.ToInt16(air);
            n = Convert.ToInt16(statu);//连接状态
            if (n == 1)
            {
                pictureBox6.BackgroundImage = Properties.Resources.已连接;
            }
            else
            {
                pictureBox6.BackgroundImage = Properties.Resources.未连接;
            }
            n = Convert.ToInt16(eltctric);
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
    }
}
