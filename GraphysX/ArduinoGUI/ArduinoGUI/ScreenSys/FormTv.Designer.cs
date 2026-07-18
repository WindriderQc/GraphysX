namespace ScreenSys
{
    partial class FormTv
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
            this.SuspendLayout();
            // 
            // FormTv
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.ClientSize = new System.Drawing.Size(784, 562);
            this.ControlBox = false;
            this.Name = "FormTv";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FormTv";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormTv_FormClosing);
            this.Load += new System.EventHandler(this.FormTv_Load);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.FormTv_KeyPress);
            this.MouseClick += new System.Windows.Forms.MouseEventHandler(this.FormTv_MouseClick);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.FormTv_MouseDown);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.FormTv_MouseUp);
            this.ResumeLayout(false);

        }

        #endregion
    }
}