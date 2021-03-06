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




#if (!defined __TESTUPDATEMULTIPLECONTACTSSTEP_H__)
#define __TESTUPDATEMULTIPLECONTACTSSTEP_H__
#include <test/testexecutestepbase.h>
class CTestUpdateMultipleContactsStep : public CTestStep
	{
public:
	CTestUpdateMultipleContactsStep();
	~CTestUpdateMultipleContactsStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();


private:
	CActiveScheduler* iScheduler;

	void OpenExistingContactL(TPtrC aValue, TInt aStorageType,TUid aContFieldUid, TUid avCardMapUid);
	void CreateAndUpdateMultipleContactsL(TPtrC aValue, TInt aStorageType,TUid aContFieldUid, TUid avCardMapUid);
	static TInt iStorage[];
	static TUid iFieldUid[];
	static TUid iVcardUid[];
	};

_LIT(KTestUpdateMultipleContactsStep,"TestUpdateMultipleContactsStep");

#endif
