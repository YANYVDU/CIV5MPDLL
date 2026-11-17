#pragma once

#ifndef CV_EXE_H
#define CV_EXE_H

namespace Exe
{
	//! Why a CanX / TryX refused; Lua gets GetReasonName().
	enum Reason
	{
		REASON_OK,
		REASON_BIN_HOOKS_OFF,
		REASON_UNSUPPORTED_EXE,
		// Reserved. Nothing produces these gates yet; crypto features will
		// use them when they start editing state that a resync could undo.
		REASON_NOT_NETWORK_GAME,
		REASON_NOT_HOST,
		REASON_UNAVAILABLE,
		REASON_NOT_UI_THREAD,
		REASON_WRONG_EVENT_CHANNEL,
		// Reserved for a future "another call is already in flight" guard.
		REASON_BUSY,
		NUM_REASONS
	};

	const char* GetReasonName(Reason eReason);

	//! Running EXE variant: "DX11", "DX9", "Tablet" or "Unknown".
	const char* GetBuildName();

	Reason CanDisableEngineYieldIconManager();
	Reason TryDisableEngineYieldIconManager();

}

#endif // CV_EXE_H
