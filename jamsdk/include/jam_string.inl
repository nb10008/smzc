

class String
{
public:
	String()
	{
		_init(0);
	}
	String(const String& str)
	{
		_init(0);
		assign(str.c_str(), str.size());
	}
	String(char ch)
	{
		_init(0);
		assign(&ch, 1);
	}
	String(const char* str)
	{
		_init(0);
		assign(str, strlen(str));
	}
	String(const char* str, size_t len)
	{
		_init(0);
		assign(str, len);
	}
	~String()
	{
		clear();
	}

	//  分配一个字符串
    String& assign(const char* str, size_t len)
	{
		if(len > _capacity)
		{
			String tmp;
			tmp._init(len);
			strncpy(tmp._buffer, str, len);
			swap(&tmp);
		}
		else
		{
			strncpy(_buffer, str, len);
			_setSize(len);
		}
		return (*this);
	}

    //  追加一个字符串到对象末尾
    String& append(const char* str, size_t len)
	{
		size_t newsize = _size + len;
		if(newsize > _capacity)
		{
			reserve(newsize + _capacity);
		}
		strncpy(_buffer + _size, str, len);
		_setSize(newsize);
		return (*this);
	}

    //  在任意位置插入一个字符串
    String& insert(const char* str, size_t len, uint32_t pos)
	{
		size_t newsize = _size + len;
		String tmp;
		tmp._init(newsize + _capacity);
		if(pos)
		{
			strncpy(tmp._buffer, _buffer, pos);
		}
		strncpy((tmp._buffer + pos), str, len);
		strncpy((tmp._buffer + pos + len), (_buffer + pos), (_size - pos));
		swap(&tmp);
		_setSize(newsize);
		return (*this);
	}

    //  重新分配缓存大小
    void reserve(size_t cap)
	{
		if(cap > _capacity)
		{
			String tmp;
			tmp._init(_size, cap);
			strncpy(tmp._buffer, _buffer, _size);
			swap(&tmp);
		}
	}

    //  交换两个字符串对象
    void swap(String* str)
	{
		size_t itmp;
		char* ptmp;
		jam_swap(itmp, str->_capacity, _capacity);
		jam_swap(itmp, str->_size, _size);
		jam_swap(ptmp, str->_buffer, _buffer);
	}

    //  清除内容释放缓存
    void clear()
	{
		if(_buffer)
		{
			DeleteArray(_buffer);
		}
		_init(0);
	}

    //  获取指定位置字符
    char at(int32_t index) const
	{
		if(!_buffer)
		{
			return 0;
		}
		return _buffer[index];
	}
    char& operator[](int32_t index)
	{
		return _buffer[index];
	}
    char operator[](int32_t index) const
	{
		return at(index);
	}

    //  分配一个字符串
    String& operator=(const char* str)
	{
		return assign(str, strlen(str));
	}
    String& operator=(const String& str)
	{
		return assign(str.c_str(), str.size());
	}
    String& operator=(char ch)
	{
		return assign(&ch, 1);
	}

    //  追加一个字符串到对象末尾
    String& operator+=(const char* str)
	{
		return append(str, strlen(str));
	}
    String& operator+=(const String& str)
	{
		return append(str.c_str(), str.size());
	}
    String& operator+=(char ch)
	{
		return append(&ch, 1);
	}

    //  合并两个字符串
    String operator+(const String& str) const
	{
		String tmp;
		tmp.reserve(size() + str.size());
		tmp += *this;
		tmp += str;
		return tmp;
	}
    String operator+(const char* str) const
	{
		size_t len = strlen(str);
		String tmp;
		tmp.reserve(size() + len);
		tmp += *this;
		tmp.append(str, len);
		return tmp;
	}
    String operator+(char ch) const
	{
		String tmp;
		tmp.reserve(size() + 1);
		tmp += *this;
		tmp += ch;
		return tmp;
	}

    //  比较两个字符串是否相等
    bool operator==(const String& str)
	{
		return compare(str.c_str());
	}
    bool operator==(const char* str)
	{
		return compare(str);
	}

    //  比较两个字符串是否不等
    bool operator!=(const String& str)
	{
		return !compare(str.c_str());
	}
    bool operator!=(const char* str)
	{
		return !compare(str);
	}

    //  转换成c风格的字符串
    operator const char*() const
	{
		return c_str();
	}

    //  获取字符串数据
    const char* c_str() const
	{
		if(!_buffer)
		{
			return "";
		}
		return _buffer;
	}

    //  获取字符串长度
    size_t size() const
	{
		return _size;
	}

    //  获取缓存长度
    size_t capacity() const
	{
		return _capacity;
	}

    //  是否为空字符串
    bool empty() const
	{
		return !_size;
	}

    //  从指定位置起查找字符串.(返回-1查找失败.返回位置从0起始)
    int32_t find(const char* lookup, uint32_t start = 0) const
	{
		char* ptr = strstr(_buffer + start, lookup);
		if(ptr)
		{
			return (int32_t)(ptr - _buffer);
		}
		return -1;
	}
    int32_t find(char ch, uint32_t start = 0) const
	{
		char* ptr = strchr(_buffer + start, ch);
		if(ptr)
		{
			return (int32_t)(ptr - _buffer);
		}
		return -1;
	}
    int32_t rfind(char ch) const
	{
		char* ptr = strrchr(_buffer, ch);
		if(ptr)
		{
			return (int32_t)(ptr - _buffer);
		}
		return -1;
	}

    //  从指定位置起替换字符串
    int32_t replace(const char* oldStr, const char* newStr, uint32_t start = 0)
	{
		int32_t count = 0;
		int32_t pos = (int32_t)start;
		size_t oldlen = strlen(oldStr);
		size_t newlen = strlen(newStr);
		String mirror;
		char* ptr = 0;

		for(;;)
		{
			if((pos = find(oldStr, pos)) < 0)
			{
				break;
			}
			reserve(_size - oldlen + newlen);
			ptr = _buffer + pos;

			mirror = ptr + oldlen;
			strcpy(ptr, newStr);
			strcpy(ptr + newlen, mirror.c_str());

			pos += (int32_t)newlen;
			_size += (newlen - oldlen);
			++count;
		}
		return count;
	}

    //  比较两个字符串
    bool compare(const char* str, bool ignoreCase = false) const
	{
		if(ignoreCase)
		{
			return (0 == strcasecmp(_buffer, str));
		}
		return (0 == strcmp(_buffer, str));
	}

    //  使用指定字符串分割当前字符串. 如果sp为 0 则重置split
    bool split(String* output, const char* sp)
	{
		if(!sp) //  重置函数
		{
			_spos = 0;
			return true;
		}
		if(_spos >= _size)
		{
			_spos = 0;
			return false;
		}

		int32_t fnd = find(sp, _spos);
		if(fnd < 0)
		{
			size_t rsize = _size - _spos;
			_spos = (uint32_t)_size;
			if(output)
			{
				*output = right(rsize);
			}
			return true;
		}

		size_t splen = strlen(sp);
		if(output)
		{
			*output = subString(_spos, ((size_t)fnd - _spos));
		}
		_spos = (uint32_t)(fnd + splen);
		return true;
	}

    //  转换为小写
    String& lowercase()
	{
		jam_lowercase(_buffer);
		return (*this);
	}

    //  转换为大写
    String& uppercase()
	{
		jam_uppercase(_buffer);
		return (*this);
	}

    //  去除左右指定字符
    String& trim(char ch = ' ')
	{
		jam_trim(_buffer, ch);
		_setSize(strlen(_buffer));
		return (*this);
	}

    //  去除左边指定字符
    String& trimLeft(char ch = ' ')
	{
		jam_trim_left(_buffer, ch);
		_setSize(strlen(_buffer));
		return (*this);
	}

    //  去除右边指定字符
    String& trimRight(char ch = ' ')
	{
		jam_trim_right(_buffer, ch);
		_setSize(strlen(_buffer));
		return (*this);
	}

    //  获取左边指定长度字符串
    String left(size_t len) const
	{
		if(len > _size)
		{
			len = _size;
		}
		return String(_buffer, len);
	}

    //  获取右边指定长度字符串
    String right(size_t len) const
	{
		if(len > _size)
		{
			len = _size;
		}
		return String(_buffer + (_size - len), len);
	}

    //  获取指定位置指定长度字符串
    String subString(uint32_t start, size_t len) const
	{
		if(start > _size)
		{
			start = (unsigned int)_size;
		}
		if(len > (_size - start))
		{
			len = (_size - start);
		}
		return String(_buffer + start, len);
	}

    //  按指定格式分配一个字符串
    String& format(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		vformat(fmt, args);
		va_end(args);
		return (*this);
	}
    String& vformat(const char* fmt, va_list argv)
	{
		int32_t len = vsnprintf(0, 0, fmt, argv);
		reserve(len);
		vsprintf(_buffer, fmt, argv);
		_setSize(len);
		return (*this);
	}


private:
	//  初始化对象
    void _init(size_t size)
	{
		_init(size, size);
	}
    void _init(size_t size, size_t cap)
	{
		if(cap)
		{
			_buffer = NewArray(char, cap + 1);
			_buffer[_size = size] = '\0';
			_capacity = cap;
		}
		else
		{
			_spos = 0;
			_capacity = _size = 0;
			_buffer = 0;
		}
	}

    //  设置字符串长度
    void _setSize(size_t sz)
	{
		if(_buffer)
		{
			_buffer[_size = sz] = '\0';
		}
	}


public:
    friend bool operator==(const char* left, const String& right)
	{
		return right.compare(left);
	}
    friend bool operator!=(const char* left, const String& right)
	{
		return !right.compare(left);
	}
    friend String operator+(const char* left, const String& right)
	{
		String tmp(left);
		tmp += right;
		return tmp;
	}


private:
	char*                       _buffer;
    size_t                      _size;
    size_t                      _capacity;
    uint32_t					_spos;
};

