#include <Novice.h>
#include"Vector3.h"
#include"Matrix4x4.h"
#include"assert.h"
#include<cmath>


const char kWindowTitle[] = "LD2B_04_コマツザキ_カガリ_タイトル";



// 表示の関数化
static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{

			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight + 20, "%6.02f", matrix.m[row][column]);

		}
	}
	Novice::ScreenPrintf(x, y, "%s", label);
}


// 関数のプロトタイプ宣言

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// X軸回転行列
Matrix4x4 MakeRotateXMatrix(const Vector3& rotate);
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(Vector3 rotate);
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(Vector3 rotate);
// XYZ合成
Matrix4x4 Multiply(const Matrix4x4& rotateX, const Matrix4x4& rotateYZ);
// アフィン変換
Matrix4x4 MakeAffineMatrix(const Matrix4x4& S, const Matrix4x4& R, const Matrix4x4& T);



// 関数の定義

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++)
	{
		result.m[i][i] = 1;
	}
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 result{};

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;

	return result;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(const Vector3& rotate)
{
	Matrix4x4 result{};

	result.m[1][1] = std::cos(rotate.x);
	result.m[1][2] = std::sin(rotate.x);
	result.m[2][1] = -std::sin(rotate.x);
	result.m[2][2] = std::cos(rotate.x);
	result.m[0][0] = 1;
	result.m[3][3] = 1;

	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(Vector3 rotate)
{
	Matrix4x4 result{};

	result.m[0][0] = std::cos(rotate.y);
	result.m[0][2] = -std::sin(rotate.y);
	result.m[1][1] = 1;
	result.m[2][0] = std::sin(rotate.y);
	result.m[2][2] = std::cos(rotate.y);
	result.m[3][3] = 1;

	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(Vector3 rotate)
{
	Matrix4x4 result{};

	result.m[0][0] = std::cos(rotate.z);
	result.m[0][1] = std::sin(rotate.z);
	result.m[1][0] = -std::sin(rotate.z);
	result.m[1][1] = std::cos(rotate.z);
	result.m[2][2] = 1;
	result.m[3][3] = 1;

	return result;
}

// XYZ合成
Matrix4x4 Multiply(const Matrix4x4& rotateX, const Matrix4x4& rotateYZ)
{
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += rotateX.m[i][k] * rotateYZ.m[k][j];
			}
		}
	}

	return result;
}

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Matrix4x4& S, const Matrix4x4& R, const Matrix4x4& T)
{
	Matrix4x4 SR{};
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				SR.m[i][j] += S.m[i][k] * R.m[k][j];
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += SR.m[i][k] * T.m[k][j];
			}
		}
	}

	return result;
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};


	// 利用するベクトル
	Vector3 translate{ 2.7f,-4.15f,1.57f };
	Vector3 scale{ 1.2f,0.79f,-2.1f };
	Vector3 rotate{ 0.4f,1.43f,-0.8f };



	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///


		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
		Matrix4x4 worldMatrix = MakeAffineMatrix(scaleMatrix, rotateXYZMatrix, translateMatrix);// アフィン変換


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		MatrixScreenPrintf(0, 0, worldMatrix, "worldMatrix");


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
