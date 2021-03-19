﻿// Direct Test V1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнен//ие программы.
//
#pragma once
#include "InputHandler.h"
#include "FirstRender.h"

#include "D3D11_Framework.h"
#include "SecondRender.h"

using namespace D3D11_Framework;

int main()
{
	Framework framework;

	Render* render = new SecondRender();
	InputListener* input = new InputHandler();

	framework.SetRender(render);
	framework.Init();
	framework.AddInputListener(input);

	framework.Run();

	framework.Close();

	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
