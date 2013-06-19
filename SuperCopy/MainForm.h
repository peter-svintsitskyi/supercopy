#pragma once

#include "JobThread.h"

namespace SuperCopy {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace System::IO;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();

			nLastJobId = 0;
			jobs = gcnew List<System::IntPtr>();
			selectedJob = 0;
			threads = gcnew List<Thread^>();

			// get the logical drives
			array<String^>^ drives = Directory::GetLogicalDrives();

			for(int i = 0; i < drives->Length; i++)
			{
				String^ name = dynamic_cast<String^>(drives->GetValue(i));
				array<wchar_t>^ trimChars = gcnew array<wchar_t>(1);
				trimChars[0] = '\\';
				TreeNode^ sourceNode = gcnew TreeNode(name->Trim(trimChars));
				sourceNode->ImageIndex = 2;
				sourceNode->SelectedImageIndex = 2;
				sourceTreeView->Nodes->Add(sourceNode);
				sourceNode->Nodes->Add(gcnew TreeNode("<dummy>"));

				TreeNode^ targetNode = gcnew TreeNode(name->Trim(trimChars));
				targetNode->ImageIndex = 2;
				targetNode->SelectedImageIndex = 2;
				targetTreeView->Nodes->Add(targetNode);
				targetNode->Nodes->Add(gcnew TreeNode("<dummy>"));
			}

			this->statsUpdateTimer->Start();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			for(int i = 0; i < jobs->Count; i++) {
				delete reinterpret_cast<Job*>((void*)jobs[i]);
			}

			if (components)
			{
				delete components;
			}
		}

	private: System::ComponentModel::IContainer^  components;

	private: System::Windows::Forms::Timer^  statsUpdateTimer;

	private: System::Windows::Forms::TreeView^  sourceTreeView;
	private: System::Windows::Forms::TreeView^  targetTreeView;
	private: System::Windows::Forms::DataGridView^  jobsDataGridView;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		int nLastJobId;
		Job *selectedJob;
		List<System::IntPtr>^ jobs;
		List<Thread^>^ threads;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  jobId;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  sourcePath;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  targetPath;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  status;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  transferRate;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  transferRateLimit;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  jobSize;
	private: System::Windows::Forms::SplitContainer^  mainSplitContainer;
	private: System::Windows::Forms::SplitContainer^  topSplitContainer;
	private: System::Windows::Forms::SplitContainer^  leftSplitContainer;
	public protected: System::Windows::Forms::GroupBox^  jobInfoGroupBox;
	private: System::Windows::Forms::Label^  selectedJobIdLabel;
	private: System::Windows::Forms::Button^  selectedJobControlButton;
	private: System::Windows::Forms::NumericUpDown^  selectedJobSpeedLimitUpDown;
	private: System::Windows::Forms::Label^  selectedJobSpeedLimitLabel;
	private: System::Windows::Forms::Label^  selectedJobCopyToLabel;
	private: System::Windows::Forms::Label^  selectedJobCopyFromLabel;
	private: System::Windows::Forms::ImageList^  treeViewImagesList;



	public protected: 

	public protected: 
	private: 
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  progress;


#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
				 System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
				 System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
				 this->statsUpdateTimer = (gcnew System::Windows::Forms::Timer(this->components));
				 this->sourceTreeView = (gcnew System::Windows::Forms::TreeView());
				 this->treeViewImagesList = (gcnew System::Windows::Forms::ImageList(this->components));
				 this->targetTreeView = (gcnew System::Windows::Forms::TreeView());
				 this->jobsDataGridView = (gcnew System::Windows::Forms::DataGridView());
				 this->jobId = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->sourcePath = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->targetPath = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->status = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->transferRate = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->transferRateLimit = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->jobSize = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->progress = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->mainSplitContainer = (gcnew System::Windows::Forms::SplitContainer());
				 this->topSplitContainer = (gcnew System::Windows::Forms::SplitContainer());
				 this->leftSplitContainer = (gcnew System::Windows::Forms::SplitContainer());
				 this->jobInfoGroupBox = (gcnew System::Windows::Forms::GroupBox());
				 this->selectedJobCopyToLabel = (gcnew System::Windows::Forms::Label());
				 this->selectedJobCopyFromLabel = (gcnew System::Windows::Forms::Label());
				 this->selectedJobSpeedLimitLabel = (gcnew System::Windows::Forms::Label());
				 this->selectedJobSpeedLimitUpDown = (gcnew System::Windows::Forms::NumericUpDown());
				 this->selectedJobControlButton = (gcnew System::Windows::Forms::Button());
				 this->selectedJobIdLabel = (gcnew System::Windows::Forms::Label());
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->jobsDataGridView))->BeginInit();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->mainSplitContainer))->BeginInit();
				 this->mainSplitContainer->Panel1->SuspendLayout();
				 this->mainSplitContainer->Panel2->SuspendLayout();
				 this->mainSplitContainer->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->topSplitContainer))->BeginInit();
				 this->topSplitContainer->Panel1->SuspendLayout();
				 this->topSplitContainer->Panel2->SuspendLayout();
				 this->topSplitContainer->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->leftSplitContainer))->BeginInit();
				 this->leftSplitContainer->Panel1->SuspendLayout();
				 this->leftSplitContainer->Panel2->SuspendLayout();
				 this->leftSplitContainer->SuspendLayout();
				 this->jobInfoGroupBox->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->selectedJobSpeedLimitUpDown))->BeginInit();
				 this->SuspendLayout();
				 // 
				 // statsUpdateTimer
				 // 
				 this->statsUpdateTimer->Tick += gcnew System::EventHandler(this, &MainForm::statsUpdateTimer_Tick);
				 // 
				 // sourceTreeView
				 // 
				 this->sourceTreeView->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->sourceTreeView->ImageIndex = 0;
				 this->sourceTreeView->ImageList = this->treeViewImagesList;
				 this->sourceTreeView->Location = System::Drawing::Point(0, 0);
				 this->sourceTreeView->Name = L"sourceTreeView";
				 this->sourceTreeView->PathSeparator = L"\\\\";
				 this->sourceTreeView->SelectedImageIndex = 0;
				 this->sourceTreeView->Size = System::Drawing::Size(211, 379);
				 this->sourceTreeView->TabIndex = 3;
				 this->sourceTreeView->BeforeExpand += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &MainForm::sourceTreeView_BeforeExpand);
				 this->sourceTreeView->ItemDrag += gcnew System::Windows::Forms::ItemDragEventHandler(this, &MainForm::sourceTreeView_ItemDrag);
				 // 
				 // treeViewImagesList
				 // 
				 this->treeViewImagesList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"treeViewImagesList.ImageStream")));
				 this->treeViewImagesList->TransparentColor = System::Drawing::Color::Transparent;
				 this->treeViewImagesList->Images->SetKeyName(0, L"file.png");
				 this->treeViewImagesList->Images->SetKeyName(1, L"folder.png");
				 this->treeViewImagesList->Images->SetKeyName(2, L"hard.png");
				 // 
				 // targetTreeView
				 // 
				 this->targetTreeView->AllowDrop = true;
				 this->targetTreeView->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->targetTreeView->ImageIndex = 0;
				 this->targetTreeView->ImageList = this->treeViewImagesList;
				 this->targetTreeView->Location = System::Drawing::Point(0, 0);
				 this->targetTreeView->Name = L"targetTreeView";
				 this->targetTreeView->PathSeparator = L"\\\\";
				 this->targetTreeView->SelectedImageIndex = 0;
				 this->targetTreeView->Size = System::Drawing::Size(231, 379);
				 this->targetTreeView->TabIndex = 4;
				 this->targetTreeView->BeforeExpand += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &MainForm::targetTreeView_BeforeExpand);
				 this->targetTreeView->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &MainForm::targetTreeView_DragDrop);
				 this->targetTreeView->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &MainForm::targetTreeView_DragEnter);
				 this->targetTreeView->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &MainForm::targetTreeView_DragOver);
				 // 
				 // jobsDataGridView
				 // 
				 this->jobsDataGridView->AllowUserToAddRows = false;
				 this->jobsDataGridView->AllowUserToDeleteRows = false;
				 dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
				 dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Control;
				 dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
					 System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
				 dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::WindowText;
				 dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
				 dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
				 dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
				 this->jobsDataGridView->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
				 this->jobsDataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
				 this->jobsDataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(8) {this->jobId, 
					 this->sourcePath, this->targetPath, this->status, this->transferRate, this->transferRateLimit, this->jobSize, this->progress});
				 dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
				 dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Window;
				 dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
					 System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
				 dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::ControlText;
				 dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
				 dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
				 dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
				 this->jobsDataGridView->DefaultCellStyle = dataGridViewCellStyle2;
				 this->jobsDataGridView->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->jobsDataGridView->Location = System::Drawing::Point(0, 0);
				 this->jobsDataGridView->MultiSelect = false;
				 this->jobsDataGridView->Name = L"jobsDataGridView";
				 this->jobsDataGridView->ReadOnly = true;
				 this->jobsDataGridView->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
				 this->jobsDataGridView->Size = System::Drawing::Size(899, 153);
				 this->jobsDataGridView->TabIndex = 5;
				 this->jobsDataGridView->RowEnter += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::jobsDataGridView_RowEnter);
				 // 
				 // jobId
				 // 
				 this->jobId->HeaderText = L"Job ID";
				 this->jobId->Name = L"jobId";
				 this->jobId->ReadOnly = true;
				 // 
				 // sourcePath
				 // 
				 this->sourcePath->HeaderText = L"Source Path";
				 this->sourcePath->Name = L"sourcePath";
				 this->sourcePath->ReadOnly = true;
				 // 
				 // targetPath
				 // 
				 this->targetPath->HeaderText = L"Target Path";
				 this->targetPath->Name = L"targetPath";
				 this->targetPath->ReadOnly = true;
				 // 
				 // status
				 // 
				 this->status->HeaderText = L"Status";
				 this->status->Name = L"status";
				 this->status->ReadOnly = true;
				 // 
				 // transferRate
				 // 
				 this->transferRate->HeaderText = L"Transfer Rate";
				 this->transferRate->Name = L"transferRate";
				 this->transferRate->ReadOnly = true;
				 this->transferRate->ToolTipText = L"Kilobyte per second";
				 // 
				 // transferRateLimit
				 // 
				 this->transferRateLimit->HeaderText = L"Transfer Rate Limit";
				 this->transferRateLimit->Name = L"transferRateLimit";
				 this->transferRateLimit->ReadOnly = true;
				 this->transferRateLimit->ToolTipText = L"Kilobyte per second";
				 // 
				 // jobSize
				 // 
				 this->jobSize->HeaderText = L"Job Size";
				 this->jobSize->Name = L"jobSize";
				 this->jobSize->ReadOnly = true;
				 // 
				 // progress
				 // 
				 this->progress->HeaderText = L"Progress";
				 this->progress->Name = L"progress";
				 this->progress->ReadOnly = true;
				 // 
				 // mainSplitContainer
				 // 
				 this->mainSplitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->mainSplitContainer->Location = System::Drawing::Point(0, 0);
				 this->mainSplitContainer->Name = L"mainSplitContainer";
				 this->mainSplitContainer->Orientation = System::Windows::Forms::Orientation::Horizontal;
				 // 
				 // mainSplitContainer.Panel1
				 // 
				 this->mainSplitContainer->Panel1->Controls->Add(this->topSplitContainer);
				 // 
				 // mainSplitContainer.Panel2
				 // 
				 this->mainSplitContainer->Panel2->Controls->Add(this->jobsDataGridView);
				 this->mainSplitContainer->Size = System::Drawing::Size(899, 536);
				 this->mainSplitContainer->SplitterDistance = 379;
				 this->mainSplitContainer->TabIndex = 6;
				 // 
				 // topSplitContainer
				 // 
				 this->topSplitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->topSplitContainer->Location = System::Drawing::Point(0, 0);
				 this->topSplitContainer->Name = L"topSplitContainer";
				 // 
				 // topSplitContainer.Panel1
				 // 
				 this->topSplitContainer->Panel1->Controls->Add(this->leftSplitContainer);
				 // 
				 // topSplitContainer.Panel2
				 // 
				 this->topSplitContainer->Panel2->Controls->Add(this->jobInfoGroupBox);
				 this->topSplitContainer->Size = System::Drawing::Size(899, 379);
				 this->topSplitContainer->SplitterDistance = 446;
				 this->topSplitContainer->TabIndex = 0;
				 // 
				 // leftSplitContainer
				 // 
				 this->leftSplitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->leftSplitContainer->Location = System::Drawing::Point(0, 0);
				 this->leftSplitContainer->Name = L"leftSplitContainer";
				 // 
				 // leftSplitContainer.Panel1
				 // 
				 this->leftSplitContainer->Panel1->Controls->Add(this->sourceTreeView);
				 // 
				 // leftSplitContainer.Panel2
				 // 
				 this->leftSplitContainer->Panel2->Controls->Add(this->targetTreeView);
				 this->leftSplitContainer->Size = System::Drawing::Size(446, 379);
				 this->leftSplitContainer->SplitterDistance = 211;
				 this->leftSplitContainer->TabIndex = 0;
				 // 
				 // jobInfoGroupBox
				 // 
				 this->jobInfoGroupBox->Controls->Add(this->selectedJobCopyToLabel);
				 this->jobInfoGroupBox->Controls->Add(this->selectedJobCopyFromLabel);
				 this->jobInfoGroupBox->Controls->Add(this->selectedJobSpeedLimitLabel);
				 this->jobInfoGroupBox->Controls->Add(this->selectedJobSpeedLimitUpDown);
				 this->jobInfoGroupBox->Controls->Add(this->selectedJobControlButton);
				 this->jobInfoGroupBox->Controls->Add(this->selectedJobIdLabel);
				 this->jobInfoGroupBox->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->jobInfoGroupBox->Location = System::Drawing::Point(0, 0);
				 this->jobInfoGroupBox->Name = L"jobInfoGroupBox";
				 this->jobInfoGroupBox->Size = System::Drawing::Size(449, 379);
				 this->jobInfoGroupBox->TabIndex = 3;
				 this->jobInfoGroupBox->TabStop = false;
				 this->jobInfoGroupBox->Text = L"Selected Job Properties";
				 // 
				 // selectedJobCopyToLabel
				 // 
				 this->selectedJobCopyToLabel->AutoSize = true;
				 this->selectedJobCopyToLabel->Location = System::Drawing::Point(12, 78);
				 this->selectedJobCopyToLabel->Name = L"selectedJobCopyToLabel";
				 this->selectedJobCopyToLabel->Size = System::Drawing::Size(50, 13);
				 this->selectedJobCopyToLabel->TabIndex = 9;
				 this->selectedJobCopyToLabel->Text = L"Copy To:";
				 // 
				 // selectedJobCopyFromLabel
				 // 
				 this->selectedJobCopyFromLabel->AutoSize = true;
				 this->selectedJobCopyFromLabel->Location = System::Drawing::Point(12, 50);
				 this->selectedJobCopyFromLabel->Name = L"selectedJobCopyFromLabel";
				 this->selectedJobCopyFromLabel->Size = System::Drawing::Size(60, 13);
				 this->selectedJobCopyFromLabel->TabIndex = 8;
				 this->selectedJobCopyFromLabel->Text = L"Copy From:";
				 // 
				 // selectedJobSpeedLimitLabel
				 // 
				 this->selectedJobSpeedLimitLabel->AutoSize = true;
				 this->selectedJobSpeedLimitLabel->Location = System::Drawing::Point(96, 113);
				 this->selectedJobSpeedLimitLabel->Name = L"selectedJobSpeedLimitLabel";
				 this->selectedJobSpeedLimitLabel->Size = System::Drawing::Size(33, 13);
				 this->selectedJobSpeedLimitLabel->TabIndex = 7;
				 this->selectedJobSpeedLimitLabel->Text = L"KiB/s";
				 // 
				 // selectedJobSpeedLimitUpDown
				 // 
				 this->selectedJobSpeedLimitUpDown->Enabled = false;
				 this->selectedJobSpeedLimitUpDown->Location = System::Drawing::Point(15, 106);
				 this->selectedJobSpeedLimitUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
				 this->selectedJobSpeedLimitUpDown->Name = L"selectedJobSpeedLimitUpDown";
				 this->selectedJobSpeedLimitUpDown->Size = System::Drawing::Size(75, 20);
				 this->selectedJobSpeedLimitUpDown->TabIndex = 6;
				 this->selectedJobSpeedLimitUpDown->ValueChanged += gcnew System::EventHandler(this, &MainForm::selectedJobSpeedLimitUpDown_ValueChanged);
				 // 
				 // selectedJobControlButton
				 // 
				 this->selectedJobControlButton->Enabled = false;
				 this->selectedJobControlButton->Location = System::Drawing::Point(15, 132);
				 this->selectedJobControlButton->Name = L"selectedJobControlButton";
				 this->selectedJobControlButton->Size = System::Drawing::Size(75, 23);
				 this->selectedJobControlButton->TabIndex = 4;
				 this->selectedJobControlButton->Text = L"Start";
				 this->selectedJobControlButton->UseVisualStyleBackColor = true;
				 this->selectedJobControlButton->Click += gcnew System::EventHandler(this, &MainForm::selectedJobControlButton_Click);
				 // 
				 // selectedJobIdLabel
				 // 
				 this->selectedJobIdLabel->AutoSize = true;
				 this->selectedJobIdLabel->Location = System::Drawing::Point(12, 25);
				 this->selectedJobIdLabel->Name = L"selectedJobIdLabel";
				 this->selectedJobIdLabel->Size = System::Drawing::Size(70, 13);
				 this->selectedJobIdLabel->TabIndex = 3;
				 this->selectedJobIdLabel->Text = L"Job ID: None";
				 // 
				 // MainForm
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(899, 536);
				 this->Controls->Add(this->mainSplitContainer);
				 this->Name = L"MainForm";
				 this->Text = L"SuperCopy";
				 this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->jobsDataGridView))->EndInit();
				 this->mainSplitContainer->Panel1->ResumeLayout(false);
				 this->mainSplitContainer->Panel2->ResumeLayout(false);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->mainSplitContainer))->EndInit();
				 this->mainSplitContainer->ResumeLayout(false);
				 this->topSplitContainer->Panel1->ResumeLayout(false);
				 this->topSplitContainer->Panel2->ResumeLayout(false);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->topSplitContainer))->EndInit();
				 this->topSplitContainer->ResumeLayout(false);
				 this->leftSplitContainer->Panel1->ResumeLayout(false);
				 this->leftSplitContainer->Panel2->ResumeLayout(false);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->leftSplitContainer))->EndInit();
				 this->leftSplitContainer->ResumeLayout(false);
				 this->jobInfoGroupBox->ResumeLayout(false);
				 this->jobInfoGroupBox->PerformLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->selectedJobSpeedLimitUpDown))->EndInit();
				 this->ResumeLayout(false);

			 }
#pragma endregion
	private: System::String^ formatJobSize(__int64 nSizeBytes) {
				 long double kB = nSizeBytes / 1024.0;
				 long double MB = kB / 1024.0;
				 long double GB = MB / 1024.0;

				 if(GB > 1.0) {
					 return String::Format("{0:0.#} GB", GB);
				 }

				 if(MB > 1.0) {
					 return String::Format("{0:0.#} MB", MB);
				 }

				 if(kB > 1.0) {
					 return String::Format("{0:0.#} kB", kB);
				 }

				 return String::Format("{0} bytes", nSizeBytes);
			 }

	private: System::String^ formatTransferRate(__int64 nBytesPerSecond) {
				 return String::Format("{0:0.#} KiB/s", nBytesPerSecond / 1024.0);
			 }

	private: System::String^ formatStatus(JobStatus status) {
				 if(status == jobStatusEstimating)
					 return gcnew String("Estimating");

				 if(status == jobStatusPaused)
					 return gcnew String("Paused");

				 if(status == jobStatusRunning)
					 return gcnew String("In Progress");

				 if(status == jobStatusFailed)
					 return gcnew String("Failed");

				 if(status == jobStatusAborted)
					 return gcnew String("Aborted");

				 return gcnew String("");
			 }

	private: System::Void statsUpdateTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
				 // populate jobs grid
				 for(int i = 0; i < jobs->Count; i++) {
					 Job *job =  reinterpret_cast<Job*>((void*)jobs[i]);

					 if(jobsDataGridView->Rows->Count == 0) {
						 selectedJob = job;
					 }

					 int nDataGridRowIndex = -1;
					 for(int j = 0; j < jobsDataGridView->Rows->Count; j++) {
						 if(Convert::ToInt32(jobsDataGridView->Rows[j]->Cells["jobId"]->Value) == job->getId()) {
							 nDataGridRowIndex = j;
							 break;
						 }
					 }

					 if(nDataGridRowIndex == -1) {
						 jobsDataGridView->Rows->Add();
						 nDataGridRowIndex = jobsDataGridView->Rows->Count - 1;
					 }

					 jobsDataGridView->Rows[nDataGridRowIndex]->Cells["jobId"]->Value = job->getId();
					 jobsDataGridView->Rows[nDataGridRowIndex]->Cells["sourcePath"]->Value = gcnew String(job->getSourcePath());
					 jobsDataGridView->Rows[nDataGridRowIndex]->Cells["targetPath"]->Value = gcnew String(job->getTargetPath());
					 jobsDataGridView->Rows[nDataGridRowIndex]->Cells["jobSize"]->Value = formatJobSize(job->getJobSizeBytes());
					 jobsDataGridView->Rows[nDataGridRowIndex]->Cells["progress"]->Value = String::Format("{0:0.#}%", job->getProgress());
					 jobsDataGridView->Rows[nDataGridRowIndex]->Cells["transferRate"]->Value = formatTransferRate(job->getTransferRateBytesPerSecond());
					 jobsDataGridView->Rows[nDataGridRowIndex]->Cells["transferRateLimit"]->Value = formatTransferRate(job->getTransferRateLimitBytesPerSecond());
					 jobsDataGridView->Rows[nDataGridRowIndex]->Cells["status"]->Value = formatStatus(job->getStatus());
				 }

				 // populate selected job controls
				 if(selectedJob != 0) {
					 selectedJobIdLabel->Text = String::Format("Job ID: {0}", selectedJob->getId());
					 selectedJobCopyFromLabel->Text = String::Format("Copy From: {0}", gcnew String(selectedJob->getSourcePath()));
					 selectedJobCopyToLabel->Text = String::Format("Copy To: {0}", gcnew String(selectedJob->getTargetPath()));
					 selectedJobSpeedLimitUpDown->Value = Convert::ToDecimal(selectedJob->getTransferRateLimitBytesPerSecond() / 1024.0);
					 selectedJobSpeedLimitUpDown->Enabled = true;

					 if(selectedJob->getStatus() == jobStatusEstimating) {
						 selectedJobControlButton->Text = gcnew String("Start");
						 selectedJobControlButton->Enabled = false;

					 } else if(selectedJob->getStatus() == jobStatusRunning) {
						 selectedJobControlButton->Text = gcnew String("Pause");
						 selectedJobControlButton->Enabled = true;

					 } else if(selectedJob->getStatus() == jobStatusPaused) {
						 selectedJobControlButton->Text = gcnew String("Start");
						 selectedJobControlButton->Enabled = true;
					 }
				 }
			 }

	private: System::Void sourceTreeView_BeforeExpand(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e) {
				 // First zap the dummy node, assuming there is one
				 if (e->Node->Nodes->Count != 0)
					 e->Node->Nodes->RemoveAt(0);

				 // Get a list of subdirectories
				 array<String^>^ dirs;
				 array<String^>^ files;

				 try
				 {
					 dirs = Directory::GetDirectories(e->Node->FullPath + L"\\");
					 files = Directory::GetFiles(e->Node->FullPath + L"\\");
				 }
				 catch(System::Exception^ pe)
				 {
					 MessageBox::Show(pe->Message, "Some Error!");
					 return;
				 }

				 // Add a child node for each one
				 for(int i=0; i<dirs->Length; i++)
				 {
					 String^ dirName = dynamic_cast<String^>(dirs->GetValue(i));
					 TreeNode^ tn = gcnew TreeNode(Path::GetFileName(dirName));
					 tn->ImageIndex = 1;
					 tn->SelectedImageIndex = 1;

					 e->Node->Nodes->Add(tn);
					 // Add a dummy child node
					 tn->Nodes->Add(gcnew TreeNode("<dummy>"));
				 }		 

				 // Add a child node for each one
				 for(int i=0; i<files->Length; i++)
				 {
					 String^ fileName = dynamic_cast<String^>(files->GetValue(i));
					 TreeNode^ tn = gcnew TreeNode(Path::GetFileName(fileName));
					 tn->ImageIndex = 0;
					 tn->SelectedImageIndex = 0;
					 e->Node->Nodes->Add(tn);
					 // Add a dummy child node
					 //tn->Nodes->Add(gcnew TreeNode("<dummy>"));
				 }		 
			 }
	private: System::Void targetTreeView_BeforeExpand(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e) {
				 // First zap the dummy node, assuming there is one
				 if (e->Node->Nodes->Count != 0)
					 e->Node->Nodes->RemoveAt(0);

				 // Get a list of subdirectories
				 array<String^>^ dirs;
				 try
				 {
					 dirs = Directory::GetDirectories(e->Node->FullPath + L"\\");
				 }
				 catch(System::Exception^ pe)
				 {
					 MessageBox::Show(pe->Message, "Some Error!");
					 return;
				 }

				 // Add a child node for each one
				 for(int i=0; i<dirs->Length; i++)
				 {
					 String^ dirName = dynamic_cast<String^>(dirs->GetValue(i));
					 TreeNode^ tn = gcnew TreeNode(Path::GetFileName(dirName));
					 tn->ImageIndex = 1;
					 tn->SelectedImageIndex = 1;
					 e->Node->Nodes->Add(tn);
					 // Add a dummy child node
					 tn->Nodes->Add(gcnew TreeNode("<dummy>"));
				 }		 
			 }
	private: System::Void sourceTreeView_ItemDrag(System::Object^  sender, System::Windows::Forms::ItemDragEventArgs^  e) {
				 targetTreeView->DoDragDrop(e->Item, DragDropEffects::Copy );
			 }
	private: System::Void targetTreeView_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
				 e->Effect = e->AllowedEffect;
			 }
	private: System::Void targetTreeView_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
				 // Retrieve the client coordinates of the drop location.
				 Point targetPoint = targetTreeView->PointToClient( Point(e->X,e->Y) );

				 // Retrieve the node at the drop location.
				 TreeNode^ targetNode = targetTreeView->GetNodeAt( targetPoint );

				 // Retrieve the node that was dragged.
				 TreeNode^ draggedNode = dynamic_cast<TreeNode^>(e->Data->GetData( TreeNode::typeid ));

				 System::Diagnostics::Debugger::Log(0, "asdf", draggedNode->FullPath);

				 // Confirm that the node at the drop location is not  
				 // the dragged node or a descendant of the dragged node. 
				 if (  !draggedNode->Equals( targetNode ) &&  !ContainsNode( draggedNode, targetNode ) )
				 {
					 // If it is a copy operation, clone the dragged node  
					 // and add it to the node at the drop location. 
					 if ( e->Effect == DragDropEffects::Copy )
					 {
						 targetNode->Nodes->Add( dynamic_cast<TreeNode^>(draggedNode->Clone()) );

						 String ^source = draggedNode->FullPath;
						 IntPtr strSource = System::Runtime::InteropServices::Marshal::StringToHGlobalAuto(source);

						 String ^target = targetNode->FullPath;
						 IntPtr strTarget = System::Runtime::InteropServices::Marshal::StringToHGlobalAuto(target);

						 // check if job with same source and target exists
						 for(int i = 0; i < jobs->Count; i++) {
							 Job *job =  reinterpret_cast<Job*>((void*)jobs[i]);
						 }

						 // add new job
						 Job * job = new Job(++nLastJobId, (LPCTSTR)strSource.ToPointer(), (LPCTSTR)strTarget.ToPointer());
						 jobs->Add(System::IntPtr((void*)job));

						 JobThread^ jt1 = gcnew JobThread(job);
						 Thread^ t1 = gcnew Thread(gcnew ThreadStart(jt1, &JobThread::operator()));
						 t1->Name = String::Format("Job #{0}", job->getId());
						 t1->Start();
						 threads->Add(t1);

						 System::Runtime::InteropServices::Marshal::FreeHGlobal(strSource);
						 System::Runtime::InteropServices::Marshal::FreeHGlobal(strTarget);
					 }

					 // Expand the node at the location  
					 // to show the dropped node.
					 targetNode->Expand();
				 }
			 }
	private: System::Void targetTreeView_DragOver(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
				 // Retrieve the client coordinates of the mouse position.
				 Point targetPoint = targetTreeView->PointToClient( Point(e->X,e->Y) );

				 // Select the node at the mouse position.
				 targetTreeView->SelectedNode = targetTreeView->GetNodeAt( targetPoint );
			 }

			 // Determine whether one node is a parent  
			 // or ancestor of a second node. 
	private: bool ContainsNode( TreeNode^ node1, TreeNode^ node2 )
			 {
				 // Check the parent node of the second node. 
				 if ( node2->Parent == nullptr )
					 return false;

				 if ( node2->Parent->Equals( node1 ) )
					 return true;

				 // If the parent node is not null or equal to the first node,  
				 // call the ContainsNode method recursively using the parent of  
				 // the second node. 
				 return ContainsNode( node1, node2->Parent );
			 }

	private: System::Void jobsDataGridView_RowEnter(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
				 for(int i = 0; i < jobs->Count; i++) {
					 Job *job =  reinterpret_cast<Job*>((void*)jobs[i]);
					 if(Convert::ToInt32(jobsDataGridView->Rows[e->RowIndex]->Cells["jobId"]->Value) == job->getId()) {
						 selectedJob = job;
						 break;
					 }
				 }
			 }

	private: System::Void selectedJobControlButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(selectedJob == 0) {
					 return;
				 }

				 if(selectedJob->getStatus() == jobStatusPaused) {
					 selectedJob->setStatus(jobStatusRunning);
					 return;
				 }

				 if(selectedJob->getStatus() == jobStatusRunning) {
					 selectedJob->setStatus(jobStatusPaused);
					 return;
				 }
			 }
	private: System::Void selectedJobSpeedLimitUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 if(selectedJob == 0) {
					 return;
				 }

				 selectedJob->setTransferRateLimitBytesPerSecond(Convert::ToInt64(selectedJobSpeedLimitUpDown->Value * 1024));
			 }
	private: System::Void MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 // stop and cleanup each job
				 for(int i = 0; i < jobs->Count; i++) {
					 Job *job =  reinterpret_cast<Job*>((void*)jobs[i]);
					 job->setStatus(jobStatusAborted);
				 }

				 bool cleaning = false;
				 do { // terminate threads only when each job is clean
					 for(int i = 0; i < jobs->Count; i++) {
						 Job *job =  reinterpret_cast<Job*>((void*)jobs[i]);
						 if(!job->isClean()) {
							 cleaning = true;
							 break;
						 }
					 }
					 Thread::Sleep(1000);
				 } while(cleaning);

				 // terminate all threads
				 for(int i = 0; i < threads->Count; i++) {
					 threads[i]->Abort();
				 }
			 }
	};
}

