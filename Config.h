//Singleton
class Config
{
private:
	Config() {}
	Config(const Config&);
	Config& operator=(Config&);
public:
	static Config& getInstance() {
		static Config  instance;
		return instance;
	}
};