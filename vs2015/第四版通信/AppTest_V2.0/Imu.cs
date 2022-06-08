using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
namespace AppTest_V2._0
{
    public partial class Imu : UserControl
    {
        int x0, x1, y0, y1, z0, z1, x21 = 0, x20 = 0, y21 = 0, y20 = 0, z21 = 0, z20 = 0;

        float[] xx = new float[12];
        int isorno = 0;
        int or = 0;
        int nt = 0;

        SaveFileDialog savefiledialog = new SaveFileDialog();//实例化打开文件控件
        StreamWriter streamwriter;
        string str = "";

        private void button13_Click(object sender, EventArgs e)
        {
            y1 = 1;
        }

        private void button12_Click(object sender, EventArgs e)
        {
            y1 = 0;
            y21 = 1;
            if (y20 == 1 && y21 == 1)
                caliy.Text = ((Convert.ToSingle(caliy1.Text) + Convert.ToSingle(caliy2.Text)) / 2).ToString();

        }

        private void button7_Click(object sender, EventArgs e)
        {
            z0 = 1;
        }

        private void button6_Click(object sender, EventArgs e)
        {
            z0 = 0;
            z20 = 1;
            if (z20 == 1 && z21 == 1)
                caliz.Text = ((Convert.ToSingle(caliz1.Text) + Convert.ToSingle(caliz2.Text)) / 2).ToString();

        }

        private void button11_Click(object sender, EventArgs e)
        {
            z1 = 1;
        }

        private void button10_Click(object sender, EventArgs e)
        {
            z1 = 0;
            z21 = 1;
            if (z20 == 1 && z21 == 1)
                caliz.Text = ((Convert.ToSingle(caliz1.Text) + Convert.ToSingle(caliz2.Text)) / 2).ToString();

        }

        private void button14_Click(object sender, EventArgs e)
        {
            

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void button15_Click(object sender, EventArgs e)//结束保存
        {
            isorno = 2;
            
            streamwriter.Close();
            str = "";
        }

        public  void savedata(float[] x)
        {


            for (int i = 0; i < 12; i++)
            {
                str = str + "  " + Convert.ToString(x[i]);
            }
            streamwriter.WriteLine(str);
            str = "";

        }

        private void timer1_Tick(object sender, EventArgs e)//决定是否开始保存
        {

            if(isorno==1)
            {
                if (nt > 0)
                {
                    savedata(xx);
                    nt--;
                }
            }
            if(isorno==2)
            {
                isorno = 0;

            }

        }

        private void button4_Click(object sender, EventArgs e)
        {
            y0 = 0;
            y20 = 1;
            if (y20 == 1 && y21 == 1)
                caliy.Text = ((Convert.ToSingle(caliy1.Text) + Convert.ToSingle(caliy2.Text)) / 2).ToString();

        }

        private void button5_Click(object sender, EventArgs e)
        {
            y0 = 1;
        }

        private void button8_Click(object sender, EventArgs e)
        {
            x1 = 0;
            x21 = 1;
            if (x20 == 1 && x21 == 1)
                calix.Text = ((Convert.ToSingle(calix1.Text) + Convert.ToSingle(calix2.Text)) / 2).ToString();

        }

        private void button9_Click(object sender, EventArgs e)
        {
            x1 = 1;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            x0 = 0;
            x20 = 1;
            if (x20 == 1 && x21 == 1)
                calix.Text = ((Convert.ToSingle(calix1.Text) + Convert.ToSingle(calix2.Text)) / 2).ToString();

        }

        public SaveFileDialog saveFileDialog1 = new SaveFileDialog();
        public Imu()
        {
            InitializeComponent();
            
        }

        private void Imu_Load(object sender, EventArgs e)
        {

        }
        
        private void button1_Click_1(object sender, EventArgs e)//保存
        {
            savefiledialog = new SaveFileDialog();//实例化打开文件控件   
            savefiledialog.Filter = "文本文件|*.txt";//可保存的文件格式
            if (savefiledialog.ShowDialog() == DialogResult.OK)
            {
                string FileName = DateTime.Now.Year.ToString() + "/" + DateTime.Now.Month.ToString() + "/" +
                  DateTime.Now.Day.ToString() + ":" + DateTime.Now.Second.ToString();
                 streamwriter = new StreamWriter(savefiledialog.FileName);//实例化保存文件控件
                isorno = 1;//开始保存的标志
            }
            //string fileName;
            //fileName = "data.txt";
            //StreamWriter sw = new StreamWriter(fileName, true);
            //string str = "";
            //for (int i = 0; i < 3; i++)
            //{
            //    str = str + " " + Convert.ToString(x[i]);
            //}
            //sw.WriteLine(str);
            //sw.Close();




                //try
                //{
                //    SaveFileDialog savefiledialog = new SaveFileDialog();//实例化打开文件控件
                //    savefiledialog.Filter = "文本文件|*.txt";//可保存的文件格式
                //    if (savefiledialog.ShowDialog() == DialogResult.OK)
                //    {
                //        string str;
                //        string FileName = DateTime.Now.Year.ToString() + "/" + DateTime.Now.Month.ToString() + "/" +
                //                          DateTime.Now.Day.ToString() + ":" + DateTime.Now.Second.ToString();
                //        StreamWriter streamwriter = new StreamWriter(savefiledialog.FileName);//实例化保存文件控件

                //        str = textBox11.Text.ToString() + " " + textBox12.Text.ToString() + " " + textBox13.Text.ToString() + " "
                //            + textBox1.Text.ToString() + "  " + textBox2.Text.ToString() + "  " + textBox3.Text.ToString() +
                //            "  " + textBox4.Text.ToString() + " " + textBox5.Text.ToString() + " " + textBox6.Text.ToString()
                //            + "  "  + textBox7.Text.ToString() + "  " +
                //            textBox8.Text.ToString() + "  " + textBox9.Text.ToString() + "  \n ";
                //        streamwriter.Write(str);
                //        streamwriter.Flush();//刷新缓存
                //        streamwriter.Close();//关闭流

                //    }
                //}
                //catch
                //{
                //    Console.WriteLine("报错：保存文件失败！");
                //}
        }

        private void button2_Click(object sender, EventArgs e)
        {
            x0 = 1;
        }
    }
}
