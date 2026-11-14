#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include <filesystem>
#include <system_error>

using namespace geode::prelude;

// Возвращает путь к пользовательскому звуку, если он выбран и существует,
// иначе путь к встроенному звуку.
static std::filesystem::path pickSound(char const* settingKey, char const* builtin) {
	auto mod = Mod::get();
	auto custom = mod->getSettingValue<std::filesystem::path>(settingKey);
	if (!custom.empty()) {
		std::error_code ec;
		if (std::filesystem::exists(custom, ec) && !ec) {
			return custom;
		}
	}
	return mod->getResourcesDir() / builtin;
}

class $modify(ClickBotLayer, GJBaseGameLayer) {
	void handleButton(bool down, int button, bool isPlayer1) {
		GJBaseGameLayer::handleButton(down, button, isPlayer1);

		auto mod = Mod::get();
		if (!mod->getSettingValue<bool>("enabled")) return;
		if (button != 1) return; // только прыжок
		if (!PlayLayer::get()) return; // не в редакторе
		if (!down && !mod->getSettingValue<bool>("release")) return;

		auto file = down
			? pickSound("click-sound", "click.wav")
			: pickSound("release-sound", "release.wav");
		auto volume = static_cast<float>(mod->getSettingValue<double>("volume"));

		FMODAudioEngine::sharedEngine()->playEffect(file.string(), 1.f, 0.f, volume);
	}
};
