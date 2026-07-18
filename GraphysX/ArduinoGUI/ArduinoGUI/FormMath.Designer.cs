namespace MathGame
{
    partial class FormMath
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
            this.hScrollBarA = new System.Windows.Forms.HScrollBar();
            this.hScrollBarB = new System.Windows.Forms.HScrollBar();
            this.hScrollBarC = new System.Windows.Forms.HScrollBar();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.hScrollBarXOffset = new System.Windows.Forms.HScrollBar();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.SuspendLayout();
            // 
            // hScrollBarA
            // 
            this.hScrollBarA.Cursor = System.Windows.Forms.Cursors.VSplit;
            this.hScrollBarA.LargeChange = 1;
            this.hScrollBarA.Location = new System.Drawing.Point(10, 30);
            this.hScrollBarA.Maximum = 10;
            this.hScrollBarA.Minimum = -10;
            this.hScrollBarA.Name = "hScrollBarA";
            this.hScrollBarA.Size = new System.Drawing.Size(300, 20);
            this.hScrollBarA.TabIndex = 0;
            this.hScrollBarA.ValueChanged += new System.EventHandler(this.hScrollBarA_ValueChanged);
            // 
            // hScrollBarB
            // 
            this.hScrollBarB.Cursor = System.Windows.Forms.Cursors.VSplit;
            this.hScrollBarB.LargeChange = 1;
            this.hScrollBarB.Location = new System.Drawing.Point(10, 70);
            this.hScrollBarB.Maximum = 10;
            this.hScrollBarB.Minimum = -10;
            this.hScrollBarB.Name = "hScrollBarB";
            this.hScrollBarB.Size = new System.Drawing.Size(300, 20);
            this.hScrollBarB.TabIndex = 1;
            this.hScrollBarB.ValueChanged += new System.EventHandler(this.hScrollBarB_ValueChanged);
            // 
            // hScrollBarC
            // 
            this.hScrollBarC.Cursor = System.Windows.Forms.Cursors.VSplit;
            this.hScrollBarC.LargeChange = 1;
            this.hScrollBarC.Location = new System.Drawing.Point(10, 115);
            this.hScrollBarC.Maximum = 10;
            this.hScrollBarC.Minimum = -10;
            this.hScrollBarC.Name = "hScrollBarC";
            this.hScrollBarC.Size = new System.Drawing.Size(300, 20);
            this.hScrollBarC.TabIndex = 2;
            this.hScrollBarC.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hScrollBarC_Scroll);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(338, 33);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(36, 20);
            this.textBox1.TabIndex = 3;
            this.textBox1.Text = "1";
            // 
            // hScrollBarXOffset
            // 
            this.hScrollBarXOffset.Cursor = System.Windows.Forms.Cursors.VSplit;
            this.hScrollBarXOffset.LargeChange = 1;
            this.hScrollBarXOffset.Location = new System.Drawing.Point(10, 158);
            this.hScrollBarXOffset.Maximum = 10;
            this.hScrollBarXOffset.Minimum = -10;
            this.hScrollBarXOffset.Name = "hScrollBarXOffset";
            this.hScrollBarXOffset.Size = new System.Drawing.Size(300, 20);
            this.hScrollBarXOffset.TabIndex = 4;
            this.hScrollBarXOffset.ValueChanged += new System.EventHandler(this.hScrollBarXOffset_ValueChanged);
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(27, 212);
            this.trackBar1.Minimum = -10;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(320, 45);
            this.trackBar1.TabIndex = 5;
            this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // FormMath
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(398, 289);
            this.Controls.Add(this.trackBar1);
            this.Controls.Add(this.hScrollBarXOffset);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.hScrollBarC);
            this.Controls.Add(this.hScrollBarB);
            this.Controls.Add(this.hScrollBarA);
            this.Name = "FormMath";
            this.Text = "FormMath";
            this.Load += new System.EventHandler(this.FormMath_Load);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.HScrollBar hScrollBarA;
        private System.Windows.Forms.HScrollBar hScrollBarB;
        private System.Windows.Forms.HScrollBar hScrollBarC;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.HScrollBar hScrollBarXOffset;
        private System.Windows.Forms.TrackBar trackBar1;
    }
}