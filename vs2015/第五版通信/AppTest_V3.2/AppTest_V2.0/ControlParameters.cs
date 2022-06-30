using System;
using System.IO;
using System.Windows.Forms;
namespace AppTest_V2._0
{
    public partial class ControlParameters : UserControl
    {

        double recRecordCount = 1;//累计收到数据的数量

        float[] x = new float[19];
        float[] y= new float[18];

        int isorno = 0;




        int nn = 0;

        public ControlParameters()
        {

            InitializeComponent();

        }

        private void ControlParameters_Load(object sender, EventArgs e)
        {

        }
        public float[] chuanzhi(float[] x0)
        {
            if (nn < 1&&x0[0]!=0)
            {
                nn=2;
                for (int i = 0; i < 18; i++)
                {
                    x[i] = x0[i];
                }
               

            }


             for(int i=0;i<18;i++)
            {
                y[i] = x0[i];
            }

            if(x[18]==1)
            {
                x[18] = 0;
            }
            if(isorno==1)
            {
                x[18] =1;
                isorno = 0;

            }
            return x;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (nn == 2)
            {
                nn++;
                textBox1.Text = x[0].ToString();
                textBox2.Text = x[1].ToString();
                textBox3.Text = x[2].ToString();
                textBox4.Text = x[3].ToString();
                textBox5.Text = x[4].ToString();
                textBox6.Text = x[5].ToString();
                textBox7.Text = x[6].ToString();
                textBox8.Text = x[7].ToString();
                textBox9.Text = x[8].ToString();
                textBox10.Text = x[9].ToString();
                textBox11.Text = x[10].ToString();
                textBox12.Text = x[11].ToString();
                textBox13.Text = x[12].ToString();
                textBox14.Text = x[13].ToString();
                textBox15.Text = x[14].ToString();
                textBox16.Text = x[15].ToString();
                textBox17.Text = x[16].ToString();
                textBox18.Text = x[17].ToString();
            }
            textBox19.Text = y[0].ToString();
            textBox20.Text = y[1].ToString();
            textBox21.Text = y[2].ToString();
            textBox22.Text = y[3].ToString();
            textBox23.Text = y[4].ToString();
            textBox24.Text = y[5].ToString();
            textBox25.Text = y[6].ToString();
            textBox26.Text = y[7].ToString();
            textBox27.Text = y[8].ToString();
            textBox28.Text = y[9].ToString();
            textBox29.Text = y[10].ToString();
            textBox30.Text = y[11].ToString();
            textBox31.Text = y[12].ToString();
            textBox32.Text = y[13].ToString();
            textBox33.Text = y[14].ToString();
            textBox34.Text = y[15].ToString();
            textBox35.Text = y[16].ToString();
            textBox36.Text = y[17].ToString();
        }

        private void button1_Click(object sender, EventArgs e)
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

                    str = textBox19.Text.ToString() + " " + textBox20.Text.ToString() + " " + textBox21.Text.ToString() + " "
                        + textBox22.Text.ToString() + "  " + textBox23.Text.ToString() + "  " +
                        textBox24.Text.ToString() +
                        "  " + textBox25.Text.ToString() + " " + textBox26.Text.ToString() + " " +
                        textBox27.Text.ToString()
                        + "  " + textBox28.Text.ToString() + "  " + textBox29.Text.ToString() + "  " +
                        textBox30.Text.ToString() + "  " + textBox31.Text.ToString() + "  " +
                        textBox32.Text.ToString() +
                        " " + textBox33.Text.ToString() + " " + textBox34.Text.ToString()
                        + textBox35.Text.ToString() + " " +
                        textBox36.Text.ToString() + "  \n ";
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

        private void button2_Click(object sender, EventArgs e)
        {
            x[18] = 1;
            isorno = 1;

           
                x[0] = Convert.ToSingle(textBox1.Text.ToString());
                x[1] = Convert.ToSingle(textBox2.Text.ToString());
                x[2] = Convert.ToSingle(textBox3.Text.ToString());
                x[3] = Convert.ToSingle(textBox4.Text.ToString());
                x[4] = Convert.ToSingle(textBox5.Text.ToString());
                x[5] = Convert.ToSingle(textBox6.Text.ToString());
                x[6] = Convert.ToSingle(textBox7.Text.ToString());
                x[7] = Convert.ToSingle(textBox8.Text.ToString());
                x[8] = Convert.ToSingle(textBox9.Text.ToString());
                x[9] = Convert.ToSingle(textBox10.Text.ToString());
                x[10] = Convert.ToSingle(textBox11.Text.ToString());
                x[11] = Convert.ToSingle(textBox12.Text.ToString());
                x[12] = Convert.ToSingle(textBox13.Text.ToString());
                x[13] = Convert.ToSingle(textBox14.Text.ToString());
                x[14] = Convert.ToSingle(textBox15.Text.ToString());
                x[15] = Convert.ToSingle(textBox16.Text.ToString());
                x[16] = Convert.ToSingle(textBox17.Text.ToString());
                x[17] = Convert.ToSingle(textBox18.Text.ToString());
        }

        private void label12_Click(object sender, EventArgs e)
        {

        }

        private void label21_Click(object sender, EventArgs e)
        {

        }

        private void timer2_Tick(object sender, EventArgs e)
        {

        }
    }
}
