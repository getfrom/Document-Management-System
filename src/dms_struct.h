#ifndef DMS_STRUCT_H
#define DMS_STRUCT_H

#include <QList>
#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

typedef struct _tag_DMS_LabelMatchItem
{
    QString lblName;
    unsigned short lblIndex;
    unsigned short lblQuoteCnt;
}DMS_LabelMatchItem, *PDMS_LabelMatchItem;

typedef struct _tag_DMS_DocBriefItem
{
    QString docName;
    QString docType;
    QString docBrief;
    QString docPath;
    QString docSize;
    QList<unsigned short> lstOfDocLabelIndices;
}DMS_DocBriefItem, *PDMS_DocBriefItem;

// only used in DMS_CndtAddRuleDialog and DMS_AddDocDialog class.
enum DMS_CndtRuleMethod { DMSRuleMethod_Include = 0x0001, DMSRuleMethod_Exclude = 0x0002,
                          DMSRuleMethod_LESS = 0x0004, DMSRuleMethod_GREATER = 0x0008 };
enum DMS_CndtRuleType { DMSRuleType_FileName = 0x0001, DMSRuleType_FileSuffix = 0x0002,
                        DMSRuleType_FileSize = 0x0004, };
enum DMS_CndtRuleCombineMode { DMSRuleCombine_AND = 0x0001, DMSRuleCombine_OR = 0x0002 };

typedef struct _tag_DMS_CndtStringMatchRuleMethod
{
    QString strMatched;
    DMS_CndtRuleMethod ruleMethod;
}DMS_CndtStringMatchRuleMethod, *PDMS_CndtStringMatchRuleMethod;

typedef struct _tag_DMS_CndtStringMatchRuleType
{
    QString strMatched;
    DMS_CndtRuleType ruleType;
    QList<DMS_CndtRuleMethod> lstMethodMatched;
}DMS_CndtStringMatchRuleType, *PDMS_CndtStringMatchRuleType;

typedef struct _tag_DMS_CndtRule
{
    DMS_CndtRuleMethod ruleMethod;
    DMS_CndtRuleType ruleType;
    QString ruleExt;
    QString ruleExt2;
}DMS_CndtRule, *PDMS_CndtRule;

typedef struct _tag_DMS_CndtAddItem
{
    QString cndtName;
    unsigned int cndtLevel; // 0 for dir, 1 or greater for rule
    DMS_CndtRule cndtRule;
    _tag_DMS_CndtAddItem *nextOrs;
    _tag_DMS_CndtAddItem *nextAnds; // for children equivalently
}DMS_CndtAddItem, *PDMS_CndtAddItem;

typedef struct _tag_DMS_StateIndexNode
{
    int curIndex;
    int trueToIndex; // -1 for failed. -2 for succeeded.
    int falseToIndex;
}DMS_StateIndexNode, *PDMS_StateIndexNode;


#endif // DMS_STRUCT_H
