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
    public partial class Gps : UserControl
    {
        public Gps()
        {
            InitializeComponent();
        }
        private void Gps_Load(object sender, EventArgs e)
        {

        }
        public void chuanzhi(float[] x)
        {
            textBox1.Text = x[0].ToString();
            textBox2.Text = x[1].ToString();
            textBox3.Text = x[2].ToString();
            textBox4.Text = x[3].ToString();
            textBox5.Text = x[4].ToString();
            textBox6.Text = x[5].ToString();
            textBox7.Text = x[6].ToString();
        }
        private void button1_Click_1(object sender, EventArgs e)
        {
            try
            {
                SaveFileDialog savefiledialog = new SaveFileDialog();//实例化打开文件控件
                savefiledialog.Filter = "文本文件|*.txt";//可保存的文件格式
                if (savefiledialog.ShowDialog() == DialogResult.OK)
                {
                    string str;
                        string FileName = DateTime.Now.Year.ToString() + "/" + DateTime.Now.Month.ToString() + "/" +
                                           DateTime.Now.Day.ToString() + ":" + DateTime.Now.Second.ToString();
                         StreamWriter streamwriter = new StreamWriter(savefiledialog.FileName);//实例化保存文件控件

                         str = textBox1.Text.ToString() + " " + textBox2.Text.ToString() + " " + textBox3.Text.ToString() + " "
                             + textBox4.Text.ToString() + "  " + textBox5.Text.ToString() + "  " + textBox6.Text.ToString() +
                             "  " + textBox7.Text.ToString() + "  \n ";
                         streamwriter.Write(str);
                         streamwriter.Flush();//刷新缓存
                         streamwriter.Close();//关闭流

                }
            }
            catch
            {
                Console.WriteLine("报错：保存文件失败！");
            }
        }
    }
}
