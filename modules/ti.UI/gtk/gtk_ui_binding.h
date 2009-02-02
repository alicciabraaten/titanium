/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */


#ifndef _GTK_UI_BINDING_H_
#define _GTK_UI_BINDING_H_

#include "../ui_module.h"

namespace ti
{
	class GtkUIBinding : public UIBinding
	{

		public:
		GtkUIBinding();

		SharedPtr<MenuItem> CreateMenu();
		void SetMenu(SharedPtr<MenuItem>);
		void SetContextMenu(SharedPtr<MenuItem>);
		void SetIcon(SharedString icon_path);
		SharedPtr<TrayItem> AddTray(SharedString icon_path,
		                            SharedBoundMethod cb);

		private:
	};
}

#endif
