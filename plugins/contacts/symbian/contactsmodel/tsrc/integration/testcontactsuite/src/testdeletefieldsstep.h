/*
* Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
* Contact: http://www.qt-project.org/legal
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/




#if (!defined __TESTDELETEFIELDSSTEP_H__)
#define __TESTDELETEFIELDSSTEP_H__
#include <test/testexecutestepbase.h>

_LIT(KTestDeleteFieldsStep,"TestDeleteFieldsStep");

class CTestDeleteFieldsStep : public CTestStep
	{
public:
	CTestDeleteFieldsStep();
	~CTestDeleteFieldsStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	CActiveScheduler* iScheduler;
	RFs iFsSession;
	void DeleteFieldsL(TPtrC aValue, TInt aStorageType,TUid aContFieldUid, TUid aFieldvCardUid);
	void DeleteFieldsNewTemplateL(TPtrC aValue, TInt aStorageType,TUid aContFieldUid, TUid aFieldvCardUid);
	static TInt iStorage[];
	static TUid iFieldUid[];
	static TUid iVcardUid[];
	};



#endif
