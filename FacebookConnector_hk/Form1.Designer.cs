namespace csharp_oauth
{
    partial class frmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btnStart = new System.Windows.Forms.Button();
            this.logBox = new System.Windows.Forms.RichTextBox();
            this.btnclear = new System.Windows.Forms.Button();
            this.labelcount = new System.Windows.Forms.Label();
            this.tmrDisp = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // btnStart
            // 
            this.btnStart.Location = new System.Drawing.Point(12, 21);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(117, 23);
            this.btnStart.TabIndex = 0;
            this.btnStart.Text = "Start";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // logBox
            // 
            this.logBox.Location = new System.Drawing.Point(12, 55);
            this.logBox.Name = "logBox";
            this.logBox.Size = new System.Drawing.Size(446, 145);
            this.logBox.TabIndex = 2;
            this.logBox.Text = "";
            // 
            // btnclear
            // 
            this.btnclear.Location = new System.Drawing.Point(135, 21);
            this.btnclear.Name = "btnclear";
            this.btnclear.Size = new System.Drawing.Size(117, 23);
            this.btnclear.TabIndex = 3;
            this.btnclear.Text = "Clear";
            this.btnclear.UseVisualStyleBackColor = true;
            this.btnclear.Click += new System.EventHandler(this.btnclear_Click);
            // 
            // labelcount
            // 
            this.labelcount.Location = new System.Drawing.Point(13, 207);
            this.labelcount.Name = "labelcount";
            this.labelcount.Size = new System.Drawing.Size(309, 23);
            this.labelcount.TabIndex = 4;
            this.labelcount.Text = "label1";
            // 
            // tmrDisp
            // 
            this.tmrDisp.Interval = 5000;
            this.tmrDisp.Tick += new System.EventHandler(this.tmrDisp_Tick);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(470, 228);
            this.Controls.Add(this.labelcount);
            this.Controls.Add(this.btnclear);
            this.Controls.Add(this.logBox);
            this.Controls.Add(this.btnStart);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "frmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "KeyCode Bridge Server";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.frmMain_FormClosed);
            this.Leave += new System.EventHandler(this.frmMain_Leave);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.RichTextBox logBox;
        private System.Windows.Forms.Button btnclear;
        private System.Windows.Forms.Label labelcount;
        private System.Windows.Forms.Timer tmrDisp;
    }
}

