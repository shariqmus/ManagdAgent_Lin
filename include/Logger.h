#ifndef LOGGER_H
#define LOGGER_H


class Logger
{
    public:
        Logger();
        virtual ~Logger();

        Log(wstring type = L"INFO", wstring message);
        LogInfo(wstring message);

    protected:

    private:
};

#endif // LOGGER_H
