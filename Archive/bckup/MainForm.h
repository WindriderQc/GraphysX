#pragma once
#include "Wrapper3D.h"

namespace BallZ2015 { 

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Description résumée de MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: ajoutez ici le code du constructeur
			//
			Application::Idle += gcnew EventHandler(this, &BallZ2015::MainForm::Application_Idle);
			Tools3D = gcnew Engine3DWrapper(this->pictureBox1->Handle, Application::StartupPath);
		}

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisées.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  buttonMove;

	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  MouseClickLabel;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  labelX;
	private: System::Windows::Forms::Label^  labelY;
	private: System::Windows::Forms::Label^  labelZ;

	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Label^  LabelName;
	private: System::Windows::Forms::Button^  buttonSavePts;
	private: System::Windows::Forms::Button^  buttonSimFormation;
	private: System::Windows::Forms::Button^  buttonSaveFormation;


	protected:

	private:
		/// <summary>
		/// Variable nécessaire au concepteur.
		/// </summary>
		Engine3DWrapper^ Tools3D;
	private: System::Windows::Forms::Button^  buttonLoadXml;
			 XMLDataWrapper^ XMLData;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette méthode avec l'éditeur de code.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->buttonMove = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->MouseClickLabel = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->labelX = (gcnew System::Windows::Forms::Label());
			this->labelY = (gcnew System::Windows::Forms::Label());
			this->labelZ = (gcnew System::Windows::Forms::Label());
			this->LabelName = (gcnew System::Windows::Forms::Label());
			this->buttonSavePts = (gcnew System::Windows::Forms::Button());
			this->buttonSimFormation = (gcnew System::Windows::Forms::Button());
			this->buttonSaveFormation = (gcnew System::Windows::Forms::Button());
			this->buttonLoadXml = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox1->Location = System::Drawing::Point(13, 13);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(650, 464);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pictureBox1_MouseDown);
			this->pictureBox1->MouseEnter += gcnew System::EventHandler(this, &MainForm::pictureBox1_MouseEnter);
			this->pictureBox1->MouseLeave += gcnew System::EventHandler(this, &MainForm::pictureBox1_MouseLeave);
			this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pictureBox1_MouseMove);
			this->pictureBox1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pictureBox1_MouseUp);
			// 
			// buttonMove
			// 
			this->buttonMove->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonMove->Location = System::Drawing::Point(864, 13);
			this->buttonMove->Name = L"buttonMove";
			this->buttonMove->Size = System::Drawing::Size(75, 23);
			this->buttonMove->TabIndex = 1;
			this->buttonMove->Text = L"Move";
			this->buttonMove->UseVisualStyleBackColor = true;
			this->buttonMove->Click += gcnew System::EventHandler(this, &MainForm::buttonMove_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &MainForm::timer1_Tick);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(704, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(60, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Mouse Pos";
			// 
			// MouseClickLabel
			// 
			this->MouseClickLabel->AutoSize = true;
			this->MouseClickLabel->Location = System::Drawing::Point(704, 45);
			this->MouseClickLabel->Name = L"MouseClickLabel";
			this->MouseClickLabel->Size = System::Drawing::Size(35, 13);
			this->MouseClickLabel->TabIndex = 3;
			this->MouseClickLabel->Text = L"label2";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(840, 454);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(99, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Selected Mesh Pos";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(707, 108);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(35, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"label3";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(748, 108);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(35, 13);
			this->label4->TabIndex = 6;
			this->label4->Text = L"label4";
			// 
			// labelX
			// 
			this->labelX->AutoSize = true;
			this->labelX->Location = System::Drawing::Point(840, 477);
			this->labelX->Name = L"labelX";
			this->labelX->Size = System::Drawing::Size(35, 13);
			this->labelX->TabIndex = 7;
			this->labelX->Text = L"label5";
			// 
			// labelY
			// 
			this->labelY->AutoSize = true;
			this->labelY->Location = System::Drawing::Point(872, 477);
			this->labelY->Name = L"labelY";
			this->labelY->Size = System::Drawing::Size(35, 13);
			this->labelY->TabIndex = 8;
			this->labelY->Text = L"label6";
			// 
			// labelZ
			// 
			this->labelZ->AutoSize = true;
			this->labelZ->Location = System::Drawing::Point(904, 477);
			this->labelZ->Name = L"labelZ";
			this->labelZ->Size = System::Drawing::Size(35, 13);
			this->labelZ->TabIndex = 9;
			this->labelZ->Text = L"label7";
			// 
			// LabelName
			// 
			this->LabelName->AutoSize = true;
			this->LabelName->Location = System::Drawing::Point(840, 428);
			this->LabelName->Name = L"LabelName";
			this->LabelName->Size = System::Drawing::Size(64, 13);
			this->LabelName->TabIndex = 10;
			this->LabelName->Text = L"Mesh Name";
			// 
			// buttonSavePts
			// 
			this->buttonSavePts->Location = System::Drawing::Point(815, 150);
			this->buttonSavePts->Name = L"buttonSavePts";
			this->buttonSavePts->Size = System::Drawing::Size(111, 24);
			this->buttonSavePts->TabIndex = 11;
			this->buttonSavePts->Text = L"Save Point";
			this->buttonSavePts->UseVisualStyleBackColor = true;
			this->buttonSavePts->Click += gcnew System::EventHandler(this, &MainForm::buttonSavePts_Click);
			// 
			// buttonSimFormation
			// 
			this->buttonSimFormation->Location = System::Drawing::Point(816, 192);
			this->buttonSimFormation->Name = L"buttonSimFormation";
			this->buttonSimFormation->Size = System::Drawing::Size(109, 25);
			this->buttonSimFormation->TabIndex = 12;
			this->buttonSimFormation->Text = L"Simulate Formation";
			this->buttonSimFormation->UseVisualStyleBackColor = true;
			this->buttonSimFormation->Click += gcnew System::EventHandler(this, &MainForm::buttonSimFormation_Click);
			// 
			// buttonSaveFormation
			// 
			this->buttonSaveFormation->Location = System::Drawing::Point(816, 234);
			this->buttonSaveFormation->Name = L"buttonSaveFormation";
			this->buttonSaveFormation->Size = System::Drawing::Size(109, 25);
			this->buttonSaveFormation->TabIndex = 13;
			this->buttonSaveFormation->Text = L"Save Formation";
			this->buttonSaveFormation->UseVisualStyleBackColor = true;
			this->buttonSaveFormation->Click += gcnew System::EventHandler(this, &MainForm::buttonSaveFormation_Click);
			// 
			// buttonLoadXml
			// 
			this->buttonLoadXml->Location = System::Drawing::Point(817, 278);
			this->buttonLoadXml->Name = L"buttonLoadXml";
			this->buttonLoadXml->Size = System::Drawing::Size(109, 25);
			this->buttonLoadXml->TabIndex = 14;
			this->buttonLoadXml->Text = L"Load Formation";
			this->buttonLoadXml->UseVisualStyleBackColor = true;
			this->buttonLoadXml->Click += gcnew System::EventHandler(this, &MainForm::buttonLoadXml_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(951, 502);
			this->Controls->Add(this->buttonLoadXml);
			this->Controls->Add(this->buttonSaveFormation);
			this->Controls->Add(this->buttonSimFormation);
			this->Controls->Add(this->buttonSavePts);
			this->Controls->Add(this->LabelName);
			this->Controls->Add(this->labelZ);
			this->Controls->Add(this->labelY);
			this->Controls->Add(this->labelX);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->MouseClickLabel);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->buttonMove);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void buttonMove_Click(System::Object^  sender, System::EventArgs^  e) {
		
	}
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		
	}
	private: System::Void Application_Idle(System::Object^  sender, System::EventArgs^  e) {

		while (Tools3D->AppStillIdle())
		{
			Tools3D->UpdateLoop();

		}
		
	}
private: System::Void pictureBox1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

			MouseClickLabel->Text = e->Location.X + ":" + e->Location.Y;
	}
private: System::Void pictureBox1_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
	Tools3D->m_TV3D->pTV->ShowWinCursor(false);
}
private: System::Void pictureBox1_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
	Tools3D->m_TV3D->pTV->ShowWinCursor(true);
}
private: System::Void pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
	Tools3D->m_Area->clBallZScene->MouseGrab();
	if (Tools3D->m_Area->clBallZScene->GetMeshClicked()){
		labelX->Text = Tools3D->m_Area->clBallZScene->GetMeshClicked()->GetPosition().x.ToString("0.0");
		labelY->Text = Tools3D->m_Area->clBallZScene->GetMeshClicked()->GetPosition().y.ToString("0.0");
		labelZ->Text = Tools3D->m_Area->clBallZScene->GetMeshClicked()->GetPosition().z.ToString("0.0");

		String^ something = gcnew String(Tools3D->m_Area->clBallZScene->GetMeshClicked()->GetMeshName());
		LabelName->Text = something;
	}
	else
		LabelName->Text = "N/A";

}
private: System::Void pictureBox1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

	Tools3D->m_Area->clBallZScene->MouseDrop();

}
private: System::Void buttonSavePts_Click(System::Object^  sender, System::EventArgs^  e) {

	Tools3D->m_Area->clBallZScene->clFormation->SavePoint();
}
private: System::Void buttonSimFormation_Click(System::Object^  sender, System::EventArgs^  e) {
	Tools3D->m_Area->clBallZScene->clFormation->Simulate();
}
private: System::Void buttonSaveFormation_Click(System::Object^  sender, System::EventArgs^  e) {
	
	Tools3D->m_Area->clBallZScene->clFormation->SaveFormation();  // sauvegarde l'array de Flyers dans clXMLData
	XMLData = gcnew XMLDataWrapper(Tools3D->m_Area->clBallZScene->clFormation->SaveFormation());
	XMLData->Serialize("C:\\test\\MyWorld.xml");

	/*
	vec.x = atof(elem->Attribute("x"));
	vec.y = atof(elem->Attribute("y"));
	vec.z = atof(elem->Attribute("z"));
	Spline.push_back(vec);
	*/
}
private: System::Void buttonLoadXml_Click(System::Object^  sender, System::EventArgs^  e) {
	XMLData = gcnew XMLDataWrapper(Tools3D->m_Area->clBallZScene->clFormation->clXMLData);
	Tools3D->m_Area->clBallZScene->clFormation->LoadFormation(XMLData->Deserialized("C:\\test\\MyWorld.xml"));
}
};
}



