#pragma once

// удал€ет указатель.
#define _DELETE(p)		{ if(p){delete (p); (p)=nullptr;} }
// удал€ет массив указателей
#define _DELETE_ARRAY(p)	{ if(p){delete[] (p); (p)=nullptr;} }
// используетс€ дл€ удалени€ интерфейсов DirectX.
#define _RELEASE(p)		{ if(p){(p)->Release(); (p)=nullptr;} }
// позвол€ет нам удобней завершить работу наших указателей, 
// то есть вызывает метод Close(), затем удал€ет сам указатель.
#define _CLOSE(p)		{ if(p){(p)->Close(); delete (p); (p)=nullptr;} }

// “екуща€ верси€ фреймворка
#define _VERSION_FRAMEWORK 228