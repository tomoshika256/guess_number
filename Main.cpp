# include <Siv3D.hpp>
#include <iostream>
#include <string>
int Post(int guess_num, int64 seed_num) {
	const URL url = U"http://tk2-242-30965.vs.sakura.ne.jp:8080/03/guessNumberHard/";
	const HashTable<String, String> headers = { { U"Content-Type", U"application/json" } };
	const std::string data = JSON{
		{ U"guess", guess_num},
		{U"seed",seed_num},
	}
	.formatUTF8();
	const FilePath saveFilePath = U"post_result.json";

	if (auto response = SimpleHTTP::Post(url, headers, data.data(), data.size(), saveFilePath))
	{
		/*

		Console << U"------";
		Console << response.getStatusLine().rtrimmed();
		Console << U"status code: " << FromEnum(response.getStatusCode());
		Console << U"------";
		Console << response.getHeader().rtrimmed();
		Console << U"------";
		*/
		if (response.isOK())
		{
			String s = TextReader{ saveFilePath }.readAll();
			if (s == U"Correct!")
				return 0;
			else if (s == U"Too low")
				return 1;
			else if (s == U"Too high")
				return 2;
		}
	}
	else
	{
		return -1;
	}
}
int henkan(String s) {
	try {
		Parse<int>(s);
	}
	catch (const ParseError& e) {
		return -1;
	}
	return Parse<int>(s);
}


void Main()
{
	int guess_num = -1;
	int64 seed_num = -1;
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });
	TextEditState guess_te;
	TextEditState seed_te;
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };

	int result = -1;
	int previou_guess_num;
	int answer = -1;
	while (System::Update())
	{
		font(U"調べたい数字").draw(20, Vec2{ 20, 20 }, ColorF{ 0.2, 0.4, 0.8 });
		SimpleGUI::TextBox(guess_te, Vec2{ 200, 20 });
		font(U"seed値:").draw(20, Vec2{ 20, 60 }, ColorF{ 0.2, 0.4, 0.8 });
		SimpleGUI::TextBox(seed_te, Vec2{ 200, 60 });
		if (guess_num <= 999 && guess_num >= 0) {
			font(U"調べる数字は{}です。"_fmt(guess_num)).draw(20, Vec2{ 20, 100 }, ColorF{ 0.2, 0.4, 0.8 });
		}
		else {
			font(U"調べたい数字の欄には0～999までの数字を入力してください").draw(20, Vec2{ 20, 100 }, ColorF{ 0.2, 0.4, 0.8 });
		}
		if (seed_num >= 0) {
			font(U"seed値は{}に設定されました"_fmt(seed_num)).draw(20, Vec2{ 20, 130 }, ColorF{ 0.2, 0.4, 0.8 });
		}
		else {
			font(U"seed値の欄には0以上の数字を入力してください").draw(20, Vec2{ 20, 130 }, ColorF{ 0.2, 0.4, 0.8 });
		}
		guess_num = henkan(guess_te.text);
		seed_num = henkan(seed_te.text);

		if (SimpleGUI::Button(U"実行", Vec2{ 20, 170 }, 120, (guess_num <= 999 && guess_num >= 0 && seed_num >= 0)))
		{
			result = Post(guess_num, seed_num);
			previou_guess_num = guess_num;
		}

		switch (result) {
		case  0:
			font(U"正解!{}は正解の数字です"_fmt(previou_guess_num)).draw(20, Vec2{ 20, 210 }, ColorF{ 0.2, 0.4, 0.8 });
			break;
		case  1:
			font(U"残念!{}は正解の数字より小さいです。"_fmt(previou_guess_num)).draw(20, Vec2{ 20, 210 }, ColorF{ 0.2, 0.4, 0.8 });
			break;
		case  2:
			font(U"残念!{}は正解の数字より大きいです。"_fmt(previou_guess_num)).draw(20, Vec2{ 20, 210 }, ColorF{ 0.2, 0.4, 0.8 });
			break;

		}
		if (SimpleGUI::Button(U"自動", Vec2{ 20, 300 }, 120, (seed_num >= 0)))
		{
			int ng = -1;
			int ok = 999;
			while (ok - ng > 1) {
				int m = (ng + ok) / 2;
				if (Post(m, seed_num) ==1)ng = m;
				else  ok = m;
			}
			answer = ok;
		}
		if(answer!=-1)
		font(U"答えは{}"_fmt(answer)).draw(20, Vec2{ 20, 340}, ColorF{ 0.2, 0.4, 0.8 });
	
	}



}
