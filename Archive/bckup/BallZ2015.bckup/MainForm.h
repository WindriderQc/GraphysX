#pragma once
#include "Screen3D.h"
#include "Wrapper3D.h"
#include "m_3D.h"
#include "SceneBuilder.h"
#include "Displayer.h"
#include "HighScoreForm.h"
#include "Tools.h"


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
			DevConsoleCreate();
			cout << "Console Loaded \n\r Initializing Windows.....";
			System::Diagnostics::Debug::WriteLine("Initialize Mainform components...");
			InitializeComponent();
			//
			//TODO: ajoutez ici le code du constructeur
			//
			Application::Idle += gcnew EventHandler(this, &BallZ2015::MainForm::Application_Idle);
			Screen3D^ Screen3DForm = gcnew Screen3D();
			Screen3DForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::Screen3DClosed);
			cout << "Windows Loaded \n\r Initializing 3DEngine.....";
			Tools3D = gcnew Engine3DWrapper(Screen3DForm->Handle, Application::StartupPath);
			//Tools3D = gcnew Engine3DWrapper(this->pictureBox1->Handle, Application::StartupPath);
			
			cout << "3DEngine Loaded \n\r Initializing Scene and Models.....";
			Builder = gcnew CSceneBuilder();
			Builder->Deserialized("..\\StockRoom\\Suzanne1.xml");  // Désérialise le XML dans le Active Scene3D
			Builder->BuildScene(Tools3D, Builder->Scene3D);
			
			Tools3D->m_Realisateur->GetActiveScene()->AddInteractions();  // PAS CHIC
			cout << "Scene Loaded";
			Screen3DForm->Show();
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
		XMLDataWrapper^ XMLData;
		CSceneBuilder^ Builder;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::TextBox^  textBoxName;
	private: System::Windows::Forms::Button^  buttonLoadXml;
			 

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette méthode avec l'éditeur de code.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->textBoxName = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// buttonMove
			// 
			this->buttonMove->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonMove->Location = System::Drawing::Point(413, 5);
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
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(253, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(60, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Mouse Pos";
			// 
			// MouseClickLabel
			// 
			this->MouseClickLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->MouseClickLabel->AutoSize = true;
			this->MouseClickLabel->Location = System::Drawing::Point(253, 37);
			this->MouseClickLabel->Name = L"MouseClickLabel";
			this->MouseClickLabel->Size = System::Drawing::Size(35, 13);
			this->MouseClickLabel->TabIndex = 3;
			this->MouseClickLabel->Text = L"label2";
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(363, 411);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(99, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Selected Mesh Pos";
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(256, 100);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(35, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"label3";
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(297, 100);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(35, 13);
			this->label4->TabIndex = 6;
			this->label4->Text = L"label4";
			// 
			// labelX
			// 
			this->labelX->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->labelX->AutoSize = true;
			this->labelX->Location = System::Drawing::Point(363, 424);
			this->labelX->Name = L"labelX";
			this->labelX->Size = System::Drawing::Size(35, 13);
			this->labelX->TabIndex = 7;
			this->labelX->Text = L"label5";
			// 
			// labelY
			// 
			this->labelY->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->labelY->AutoSize = true;
			this->labelY->Location = System::Drawing::Point(404, 424);
			this->labelY->Name = L"labelY";
			this->labelY->Size = System::Drawing::Size(35, 13);
			this->labelY->TabIndex = 8;
			this->labelY->Text = L"label6";
			// 
			// labelZ
			// 
			this->labelZ->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->labelZ->AutoSize = true;
			this->labelZ->Location = System::Drawing::Point(439, 424);
			this->labelZ->Name = L"labelZ";
			this->labelZ->Size = System::Drawing::Size(35, 13);
			this->labelZ->TabIndex = 9;
			this->labelZ->Text = L"label7";
			// 
			// LabelName
			// 
			this->LabelName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->LabelName->AutoSize = true;
			this->LabelName->Location = System::Drawing::Point(363, 398);
			this->LabelName->Name = L"LabelName";
			this->LabelName->Size = System::Drawing::Size(64, 13);
			this->LabelName->TabIndex = 10;
			this->LabelName->Text = L"Mesh Name";
			// 
			// buttonSavePts
			// 
			this->buttonSavePts->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->buttonSavePts->Location = System::Drawing::Point(364, 155);
			this->buttonSavePts->Name = L"buttonSavePts";
			this->buttonSavePts->Size = System::Drawing::Size(111, 24);
			this->buttonSavePts->TabIndex = 11;
			this->buttonSavePts->Text = L"Save Point";
			this->buttonSavePts->UseVisualStyleBackColor = true;
			this->buttonSavePts->Click += gcnew System::EventHandler(this, &MainForm::buttonSavePts_Click);
			// 
			// buttonSimFormation
			// 
			this->buttonSimFormation->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->buttonSimFormation->Location = System::Drawing::Point(365, 197);
			this->buttonSimFormation->Name = L"buttonSimFormation";
			this->buttonSimFormation->Size = System::Drawing::Size(109, 25);
			this->buttonSimFormation->TabIndex = 12;
			this->buttonSimFormation->Text = L"Simulate Formation";
			this->buttonSimFormation->UseVisualStyleBackColor = true;
			this->buttonSimFormation->Click += gcnew System::EventHandler(this, &MainForm::buttonSimFormation_Click);
			// 
			// buttonSaveFormation
			// 
			this->buttonSaveFormation->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->buttonSaveFormation->Location = System::Drawing::Point(365, 239);
			this->buttonSaveFormation->Name = L"buttonSaveFormation";
			this->buttonSaveFormation->Size = System::Drawing::Size(109, 25);
			this->buttonSaveFormation->TabIndex = 13;
			this->buttonSaveFormation->Text = L"Save Formation";
			this->buttonSaveFormation->UseVisualStyleBackColor = true;
			this->buttonSaveFormation->Click += gcnew System::EventHandler(this, &MainForm::buttonSaveFormation_Click);
			// 
			// buttonLoadXml
			// 
			this->buttonLoadXml->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->buttonLoadXml->Location = System::Drawing::Point(366, 283);
			this->buttonLoadXml->Name = L"buttonLoadXml";
			this->buttonLoadXml->Size = System::Drawing::Size(109, 25);
			this->buttonLoadXml->TabIndex = 14;
			this->buttonLoadXml->Text = L"Load Formation";
			this->buttonLoadXml->UseVisualStyleBackColor = true;
			this->buttonLoadXml->Click += gcnew System::EventHandler(this, &MainForm::buttonLoadXml_Click);
			// 
			// button1
			// 
			this->button1->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->button1->Location = System::Drawing::Point(254, 270);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(92, 44);
			this->button1->TabIndex = 15;
			this->button1->Text = L"Deserialize Scene3D";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->button2->Location = System::Drawing::Point(254, 220);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(92, 44);
			this->button2->TabIndex = 16;
			this->button2->Text = L"Serialize Scene3D";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->button3->Location = System::Drawing::Point(254, 323);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(91, 24);
			this->button3->TabIndex = 17;
			this->button3->Text = L"Clear Scene";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MainForm::button3_Click);
			// 
			// button4
			// 
			this->button4->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->button4->Location = System::Drawing::Point(259, 363);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(69, 61);
			this->button4->TabIndex = 18;
			this->button4->Text = L"PhysCube";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MainForm::button4_Click);
			// 
			// textBoxName
			// 
			this->textBoxName->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->textBoxName->Location = System::Drawing::Point(337, 363);
			this->textBoxName->Name = L"textBoxName";
			this->textBoxName->Size = System::Drawing::Size(152, 20);
			this->textBoxName->TabIndex = 19;
			this->textBoxName->Text = L"name";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(494, 512);
			this->Controls->Add(this->textBoxName);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
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
			this->Location = System::Drawing::Point(700, 0);
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"MainForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void buttonMove_Click(System::Object^  sender, System::EventArgs^  e) {
	cout << "TestClick";
		
}
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		
}

private: System::Void Application_Idle(System::Object^  sender, System::EventArgs^  e) {

	while (Tools3D->AppStillIdle())
	{
		Tools3D->UpdateLoop();
	}


}
private: System::Void Screen3DClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
	this->Close();

}
private: System::Void pictureBox1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

			MouseClickLabel->Text = e->Location.X + ":" + e->Location.Y;

	}
private: System::Void pictureBox1_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
	Tools3D->m_TV3D->pTV->ShowWinCursor(false);

}
private: System::Void pictureBox1_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
	Tools3D->m_TV3D->pTV->ShowWinCursor(true);
	
	// Stop the ball control here !!!!!!!!!!!!!!
}
private: System::Void pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
	/*
	Tools3D->m_Scene->MouseGrab();
	if (Tools3D->m_Scene->GetMeshClicked()){
		labelX->Text = Tools3D->m_Scene->GetMeshClicked()->GetPosition().x.ToString("0.0");
		labelY->Text = Tools3D->m_Scene->GetMeshClicked()->GetPosition().y.ToString("0.0");
		labelZ->Text = Tools3D->m_Scene->GetMeshClicked()->GetPosition().z.ToString("0.0");

		String^ something = gcnew String(Tools3D->m_Scene->GetMeshClicked()->GetMeshName());
		LabelName->Text = something;
		delete(something);
	}
	else
		LabelName->Text = "N/A";
		*/


}
private: System::Void pictureBox1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

	//Tools3D->m_Scene->MouseDrop();
}	 
private: System::Void buttonSavePts_Click(System::Object^  sender, System::EventArgs^  e) {

	Tools3D->m_Realisateur->GetActiveScene()->clFormation->SavePoint();
}
private: System::Void buttonSimFormation_Click(System::Object^  sender, System::EventArgs^  e) {
	Tools3D->m_Realisateur->GetActiveScene()->clFormation->Simulate();
}
private: System::Void buttonSaveFormation_Click(System::Object^  sender, System::EventArgs^  e) {
	
	Tools3D->m_Realisateur->GetActiveScene()->clFormation->SaveFormation();  // sauvegarde l'array de Flyers dans clXMLData
	XMLData = gcnew XMLDataWrapper(Tools3D->m_Realisateur->GetActiveScene()->clFormation->SaveFormation());
	XMLData->Serialize("C:\\test\\MyWorld.xml");

	/*
	vec.x = atof(elem->Attribute("x"));
	vec.y = atof(elem->Attribute("y"));
	vec.z = atof(elem->Attribute("z"));
	Spline.push_back(vec);
	*/
}
private: System::Void buttonLoadXml_Click(System::Object^  sender, System::EventArgs^  e) {
	XMLData = gcnew XMLDataWrapper(Tools3D->m_Realisateur->GetActiveScene()->clFormation->clXMLData);
	Tools3D->m_Realisateur->GetActiveScene()->LoadFormation(XMLData->Deserialized("C:\\test\\MyWorld.xml"));
}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

	Builder->Deserialized("c:\\test\\MyWorld.xml");  // Désérialise le XML dans le Active Scene3D
	Builder->BuildScene(Tools3D, Builder->Scene3D);
	Tools3D->m_Realisateur->GetActiveScene()->AddInteractions();  // PAS CHIC
}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	Builder->Serialize("c:\\test\\MyWorld.xml", Builder->Scene3D, Tools3D);
}
private: System::Void MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
}
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
	Builder->ClearScene3D(Tools3D);
}
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {

	CScene3D^ Scene2Add = gcnew CScene3D();
	Scene2Add->ObjList = gcnew List<Object3D^>();
	Scene2Add->ObjList->Add(gcnew Object3D());
	
	for each(Object3D^ Obj in Scene2Add->ObjList){
		Obj->Type = PHYSICCUBE;
		Obj->Name = textBoxName->Text + Builder->Scene3D->ObjList->Count; // "CubeAddedTest";  // TROUVER UNE FACON D'EMPECHER UN DOUBLON
		Obj->PathToMesh = "";
		Obj->TextureName = "TwoWay";
		Obj->Pos->x = 20;
		Obj->Pos->y = 10;
		Obj->Pos->z = 0;
		Obj->Scale->x = 1;
		Obj->Scale->y = 1;
		Obj->Scale->z = 1;
		Obj->bEnable = true;
		Obj->masse = NULL;
		Obj->iNewtonMat = -1;

		Builder->Scene3D->ObjList->Add(Obj);
	}


	Builder->BuildScene(Tools3D, Scene2Add);
}
private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {


}
};
}



