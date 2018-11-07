/******************************************************************************
 Copyright:      1999-2013, iFLYTEK Co., Ltd.
 File name:      CmdTextProgressBar.h
 Description:    ProgressBar in linux terminal
 Author:         pffang
 Version:        1.0
 Date:           2013-9-24
 History:        2013-9-24  Created
 *******************************************************************************/

#ifndef CMDTEXTPROGRESSBAR_H_
#define CMDTEXTPROGRESSBAR_H_
#include "VACommon.h"

#if VA_GNUC && !VA_ANDROID
namespace VA {

void CmdTextProgressBar(int pct);

} /* namespace VA */
#endif

#endif /* CMDTEXTPROGRESSBAR_H_ */
