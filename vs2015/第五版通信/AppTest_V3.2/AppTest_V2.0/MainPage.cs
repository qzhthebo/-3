using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace AppTest_V2._0
{
    public partial class MainPage : UserControl
    {
        DateTime dt_begin;
        DateTime dt_now;

        Queue<float> Q1 = new Queue<float>(100000);
        Queue<float> Q2 = new Queue<float>(100000);
        Queue<float> Q3 = new Queue<float>(100000);

        float n1 = 0;
        int n2 = 9,n3=0;//修改参数位置

        float[] x = new float[12];//IMU收
        float[] y = new float[19];//收PID

        Random r = new Random();
        int temp;
        long num = 0;//传输次数
        int cag=0;

        long t2=0, t1 = 0;

        TimeSpan ts;
        public MainPage()
        {
            InitializeComponent();
            InitChart();
        }
        public void InitChart()
        {

            Series pitch = chart2.Series[0];//定义对象实例化
            Series kp = new Series("Series2");
            chart2.Series.Add(kp);
            Series gyro_x = new Series("Series3");
            chart2.Series.Add(gyro_x);

            chart2.ChartAreas.Add("ChartArea2");
            ChartArea chartArea2 = chart2.ChartAreas[1];

            chartArea2.Position.Width = 0;
            chartArea2.Position.Height = 0;//比例？？？？？？

            // 画样条曲线（Spline）
            kp.ChartType = SeriesChartType.Spline;
            // 线宽2个像素
            kp.BorderWidth = 2;
            // 线的颜色：红色
            kp.Color = System.Drawing.Color.Red;
            // 图示上的文字            
            //kp.IsVisibleInLegend = false;   //隐藏图示上的文字          
            // 设置显示范围


            // 画样条曲线（Spline）
            pitch.ChartType = SeriesChartType.Spline;
            // 线宽2个像素
            pitch.BorderWidth = 2;
            // 线的颜色：红色
            pitch.Color = System.Drawing.Color.Black;
            // 图示上的文字            


            // 画样条曲线（Spline）
            gyro_x.ChartType = SeriesChartType.Spline;
            // 线宽2个像素
            gyro_x.BorderWidth = 2;
            // 线的颜色：红色
            gyro_x.Color = System.Drawing.Color.Green;
            // 图示上的文字            




        }
        public void Draw()
        {
            num++;
            t2 = DateTime.Now.Ticks;
            this.chart2.Series[0].Points.Clear();
            this.chart2.Series[1].Points.Clear();
            this.chart2.Series[2].Points.Clear();

            if (Q1.Count > 100)
            {
                Q2.Dequeue();
                Q1.Dequeue();
                Q3.Dequeue();
            }
                
            for (int i = 0; i < Q2.Count; i++)
            {
                chart2.Series[1].Points.AddXY(num- (Q2.Count-i-1), Q2.ElementAt(i));
                chart2.Series[0].Points.AddXY(num - (Q1.Count - i-1), Q1.ElementAt(i));
                chart2.Series[2].Points.AddXY(num - (Q3.Count - i-1), Q3.ElementAt(i));
            }
            //textBox1.Text = ((t2 - t1) / 10000000).ToString();
        }


        private void MainPage_Load(object sender, EventArgs e)
        {

        }
        public float[] chuanzhi(float []x0, float[] y0)
        {
            int i = 0;
            for(i=0;i<12;i++)
            {
                x[i] = x0[i];
            }
            for (i = 0; i < 18; i++)
            {
                y[i] = y0[i];
            }


            Q1.Enqueue(x[(n2-9)/3]);
            Q2.Enqueue(x[7]);
            Q3.Enqueue(y[n2]);
            if(y[18]==1)
            {
                n3--;
                y[n2] = Convert.ToSingle(textBox2.Text.ToString());
            }
            if(n3==0)
            {
                y[18] = 0;
            }

            //textBox1.AppendText(x.ToString()+"\n");
            //textBox1.ScrollToCaret();
            return y;
        }


        private void button1_Click(object sender, EventArgs e)
        {
            textBox3.Text = n2.ToString();
            if (cag == 0)
            {
                timer1.Enabled = true;
                dt_begin = System.DateTime.Now;
                t1 = DateTime.Now.Ticks;
                cag = 1;
            }
            else
            {
                timer1.Enabled = false;
                cag = 0;
            }
        }

        private void chart1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            n2 = Convert.ToInt16(textBox3.Text.ToString());
            
            y[18] = 1;
            n3 = 3;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Draw();
            textBox1.Text = y[n2].ToString();
            dt_now = System.DateTime.Now;
            ts = (dt_now - dt_begin);
            label2.Text = "系统运行时长：" + ts.Days + " 天 " +
                ts.Hours + " 小时 " + ts.Minutes + " 分钟 " + ts.Seconds + " 秒";
            //label1.Text = dt_now.ToString();
        }
    }
}
