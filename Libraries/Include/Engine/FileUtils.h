#pragma once

#if 0
enum FileMode : uint8
{
	Write,
	Read,
};

class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	void Open(wstring filePath, FileMode mode);

	template<typename T>
	void Write(const T& data)
	{
		DWORD numOfBytes = 0;
		assert(::WriteFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}

	template<> // �� template<> : template<typename T> (��)���� ���ܵǴ� Ư�� Ÿ������ ���� (���⼭�� string)
	void Write<string>(const string& data)
	{
		return Write(data);
	}

	void Write(void* data, uint32 dataSize);
	void Write(const string& data);

	template<typename T>
	void Read(OUT T& data)
	{
		DWORD numOfBytes = 0;
		assert(::ReadFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}

	template<typename T>
	T Read()
	{
		T data;
		Read(data);
		return data;
	}

	//void Read(void** data, uint32 dataSize); // �� ���� �Ǽ��ε�. void** �� �ʿ� ����
	void Read(void* data, uint32 dataSize);

	void Read(OUT string& data);
	/* �� Write ������ ���ø� Ư��ȭ template<> �� ����� �ݸ�, ���⼭�� ������� �ʾҴ�. Read������ �����ε� 
		  ���. �켱������ �����ε� > Ư��ȭ (template<>) > �Ϲ����ø� template<typename T>  */

private:
	HANDLE _handle = INVALID_HANDLE_VALUE;
};
#endif















































































#if 1 // Second
enum FileMode : uint8
{
	Write,
	Read,
};

class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	void Open(wstring filePath, FileMode mode);

	template<typename T>
	void Write(const T& data)
	{
		DWORD numOfBytes = 0;
		assert(::WriteFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}

	template<>
	void Write<string>(const string& data)
	{
		return Write(data);
	}

	void Write(void* data, uint32 dataSize);
	void Write(const string& data);

	template<typename T>
	void Read(OUT T& data)
	{
		DWORD numOfBytes = 0;
		assert(::ReadFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}

	template<typename T>
	T Read()
	{
		T data;
		Read(data);
		return data;
	}

	void Read(void* data, uint32 dataSize);

	void Read(OUT string& data);


private:
	HANDLE _handle = INVALID_HANDLE_VALUE;
};
#endif // Second