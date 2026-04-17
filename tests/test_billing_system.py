#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
计费管理系统测试脚本
测试项目的所有功能，包括：
1. 添加卡片
2. 查询卡片
3. 上机
4. 下机
5. 充值
6. 退费
7. 查询统计
8. 注销卡
9. 隐藏菜单功能（设置费率、查看当前费率、删除卡）
"""

import subprocess
import os
import sys
import time
import random
import string
from datetime import datetime

class BillingSystemTester:
    def __init__(self, executable_path):
        self.executable_path = executable_path
        self.test_results = []
        self.passed = 0
        self.failed = 0

    def run_test(self, test_name, inputs, expected_outputs=None):
        """
        运行单个测试用例
        :param test_name: 测试名称
        :param inputs: 输入列表
        :param expected_outputs: 预期输出列表（可选）
        :return: 测试是否通过
        """
        try:
            # 启动程序
            process = subprocess.Popen(
                [self.executable_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1
            )

            # 发送输入
            for input_str in inputs:
                process.stdin.write(input_str + "\n")
                process.stdin.flush()
                time.sleep(0.1)  # 给程序一些时间处理

            # 获取输出
            output, error = process.communicate(timeout=10)

            # 检查输出
            test_passed = True
            if expected_outputs:
                for expected in expected_outputs:
                    if expected not in output:
                        test_passed = False
                        break

            # 记录结果
            self.test_results.append({
                'name': test_name,
                'passed': test_passed,
                'output': output,
                'error': error
            })

            if test_passed:
                self.passed += 1
                print(f"✓ {test_name}")
            else:
                self.failed += 1
                print(f"✗ {test_name}")
                if expected_outputs:
                    print(f"  预期输出包含: {expected_outputs}")
                print(f"  实际输出: {output[:200]}...")

            return test_passed

        except subprocess.TimeoutExpired:
            process.kill()
            self.failed += 1
            self.test_results.append({
                'name': test_name,
                'passed': False,
                'output': '',
                'error': 'Timeout'
            })
            print(f"✗ {test_name} (超时)")
            return False
        except Exception as e:
            self.failed += 1
            self.test_results.append({
                'name': test_name,
                'passed': False,
                'output': '',
                'error': str(e)
            })
            print(f"✗ {test_name} (异常: {str(e)})")
            return False

    def generate_random_username(self, length=10):
        """生成随机用户名"""
        return ''.join(random.choices(string.ascii_lowercase + string.digits, k=length))

    def generate_random_password(self, length=6):
        """生成随机密码（包含字母和数字）"""
        letters = ''.join(random.choices(string.ascii_lowercase, k=length-1))
        digit = random.choice(string.digits)
        password = letters + digit
        return ''.join(random.sample(password, len(password)))

    def test_add_card(self):
        """测试添加卡片功能"""
        print("\n测试添加卡片功能...")

        # 测试用例1: 正常添加卡片
        username = self.generate_random_username()
        password = self.generate_random_password()
        self.run_test(
            f"添加卡片 - {username}",
            ["1", username, password, "0"],
            ["卡创建成功", "卡添加成功"]
        )

        # 测试用例2: 添加已存在的用户名
        # 由于程序会提示重新输入，我们需要提供一个不同的用户名
        different_username = self.generate_random_username()
        self.run_test(
            f"添加已存在的用户名 - {username}",
            ["1", username, different_username, password, "0"],
            ["用户名已存在", "卡创建成功"]
        )

        # 测试用例3: 添加过长的用户名
        long_username = "a" * 20
        valid_username = self.generate_random_username()
        self.run_test(
            "添加过长的用户名",
            ["1", long_username, valid_username, password, "0"],
            ["用户名过长", "卡创建成功"]
        )

        # 测试用例4: 添加纯数字密码
        self.run_test(
            "添加纯数字密码",
            ["1", self.generate_random_username(), "123456", self.generate_random_password(), "0"],
            ["密码不能为纯数字", "卡创建成功"]
        )

        # 测试用例5: 添加过长的密码
        long_password = "a" * 10
        valid_password = self.generate_random_password()
        self.run_test(
            "添加过长的密码",
            ["1", self.generate_random_username(), long_password, valid_password, "0"],
            ["密码过长", "卡创建成功"]
        )

    def test_query_card(self):
        """测试查询卡片功能"""
        print("\n测试查询卡片功能...")

        # 创建测试卡片
        username = self.generate_random_username()
        password = self.generate_random_password()

        # 测试用例6: 查询存在的卡片
        self.run_test(
            f"查询存在的卡片 - {username}",
            ["1", username, password, "2", username, password, "0"],
            ["卡创建成功", "卡添加成功", "密码正确", f"用户：{username}"]
        )

        # 测试用例7: 查询不存在的卡片
        self.run_test(
            "查询不存在的卡片",
            ["2", "nonexistent", "0"],
            ["未找到包含 'nonexistent' 的用户"]
        )

        # 测试用例8: 查询卡片密码错误
        self.run_test(
            "查询卡片密码错误",
            ["2", username, "wrongpassword", "0"],
            ["密码错误，请重新输入！"]
        )

        # 测试用例9: 模糊查询
        partial_username = username[:5]
        self.run_test(
            f"模糊查询 - {partial_username}",
            ["2", partial_username, "1", password, "0"],
            ["找到 1 个用户"]
        )

    def test_logon(self):
        """测试上机功能"""
        print("\n测试上机功能...")

        # 创建测试卡片
        username = self.generate_random_username()
        password = self.generate_random_password()

        # 测试用例10: 正常上机
        # 需要先充值，因为上机需要余额不少于2元
        self.run_test(
            f"正常上机 - {username}",
            ["1", username, password, "5", username, password, "10", "3", username, password, "0"],
            ["卡创建成功", "卡添加成功", "充值成功", "上机成功"]
        )

        # 测试用例11: 上机时余额不足
        low_balance_username = self.generate_random_username()
        self.run_test(
            f"上机时余额不足 - {low_balance_username}",
            ["1", low_balance_username, password, "3", low_balance_username, password, "0"],
            ["余额不足"]
        )

        # 测试用例12: 重复上机
        self.run_test(
            f"重复上机 - {username}",
            ["3", username, password, "0"],
            ["已在上机状态"]
        )

    def test_settle(self):
        """测试下机功能"""
        print("\n测试下机功能...")

        # 创建测试卡片并上机
        username = self.generate_random_username()
        password = self.generate_random_password()

        # 测试用例13: 正常下机
        self.run_test(
            f"正常下机 - {username}",
            ["1", username, password, "5", username, password, "100", "3", username, password, "4", username, password, "0"],
            ["卡创建成功", "卡添加成功", "充值成功", "上机成功", "下机成功"]
        )

        # 测试用例14: 下机时卡片不存在
        self.run_test(
            "下机时卡片不存在",
            ["4", "nonexistent", password, "0"],
            ["下机失败"]
        )

        # 测试用例15: 下机时余额不足
        # 创建一个新卡片，上机，等待一段时间，然后尝试下机
        low_balance_username = self.generate_random_username()
        low_balance_password = self.generate_random_password()
        self.run_test(
            "下机时余额不足",
            ["1", low_balance_username, low_balance_password, "5", low_balance_username, low_balance_password, "10", "3", low_balance_username, low_balance_password, "4", low_balance_username, low_balance_password, "0"],
            ["卡创建成功", "卡添加成功", "充值成功", "上机成功", "下机成功"]
        )

    def test_add_money(self):
        """测试充值功能"""
        print("\n测试充值功能...")

        # 创建测试卡片
        username = self.generate_random_username()
        password = self.generate_random_password()

        # 测试用例16: 正常充值
        self.run_test(
            f"正常充值 - {username}",
            ["1", username, password, "5", username, password, "100", "0"],
            ["卡创建成功", "卡添加成功", "充值成功"]
        )

        # 测试用例17: 充值金额为负数
        self.run_test(
            "充值金额为负数",
            ["5", username, password, "-10", "0"],
            ["充值金额必须大于0"]
        )

        # 测试用例18: 充值卡片不存在
        self.run_test(
            "充值卡片不存在",
            ["5", "nonexistent", password, "100", "0"],
            ["充值失败"]
        )

        # 测试用例19: 充值后查询余额
        # 创建一个新卡片，充值，然后查询余额
        query_balance_username = self.generate_random_username()
        query_balance_password = self.generate_random_password()
        self.run_test(
            "充值后查询余额",
            ["1", query_balance_username, query_balance_password, "5", query_balance_username, query_balance_password, "100", "2", query_balance_username, query_balance_password, "0"],
            ["卡创建成功", "卡添加成功", "充值成功", "余额"]
        )

    def test_refund_money(self):
        """测试退费功能"""
        print("\n测试退费功能...")

        # 创建测试卡片并充值
        username = self.generate_random_username()
        password = self.generate_random_password()

        # 测试用例20: 正常退费
        self.run_test(
            f"正常退费 - {username}",
            ["1", username, password, "5", username, password, "100", "6", username, password, "0"],
            ["卡创建成功", "卡添加成功", "充值成功", "退费成功"]
        )

        # 测试用例21: 退费卡片不存在
        self.run_test(
            "退费卡片不存在",
            ["6", "nonexistent", password, "0"],
            ["退费失败"]
        )

        # 测试用例22: 退费后查询余额
        # 创建一个新卡片，充值，退费，然后查询余额
        refund_query_username = self.generate_random_username()
        refund_query_password = self.generate_random_password()
        self.run_test(
            "退费后查询余额",
            ["1", refund_query_username, refund_query_password, "5", refund_query_username, refund_query_password, "100", "6", refund_query_username, refund_query_password, "2", refund_query_username, refund_query_password, "0"],
            ["卡创建成功", "卡添加成功", "充值成功", "退费成功", "余额"]
        )

    def test_statistics(self):
        """测试查询统计功能"""
        print("\n测试查询统计功能...")

        # 创建测试卡片并进行一些操作
        username = self.generate_random_username()
        password = self.generate_random_password()

        # 测试用例23: 查询当日统计
        self.run_test(
            f"查询当日统计 - {username}",
            ["1", username, password, "5", username, password, "100", "3", username, password, "4", username, password, "7", "0"],
            ["卡创建成功", "卡添加成功", "充值成功", "上机成功", "下机成功", "当日营收"]
        )

    def test_annual_card(self):
        """测试注销卡功能"""
        print("\n测试注销卡功能...")

        # 创建测试卡片
        username = self.generate_random_username()
        password = self.generate_random_password()

        # 测试用例24: 正常注销卡
        self.run_test(
            f"正常注销卡 - {username}",
            ["1", username, password, "8", username, password, "0"],
            ["卡创建成功", "卡添加成功", "注销卡成功"]
        )

        # 测试用例25: 注销不存在的卡
        self.run_test(
            "注销不存在的卡",
            ["8", "nonexistent", password, "0"],
            ["注销卡失败"]
        )

        # 测试用例26: 注销后查询卡片
        # 创建一个新卡片，注销，然后查询卡片
        annual_query_username = self.generate_random_username()
        annual_query_password = self.generate_random_password()
        self.run_test(
            "注销后查询卡片",
            ["1", annual_query_username, annual_query_password, "8", annual_query_username, annual_query_password, "2", annual_query_username, "0"],
            ["卡创建成功", "卡添加成功", "注销卡成功", "未找到包含"]
        )

    def test_hidden_menu(self):
        """测试隐藏菜单功能"""
        print("\n测试隐藏菜单功能...")

        # 测试用例27: 进入隐藏菜单
        self.run_test(
            "进入隐藏菜单",
            ["999", "0", "0"],
            ["隐藏菜单"]
        )

        # 测试用例28: 设置费率
        self.run_test(
            "设置费率",
            ["999", "1", "60", "5", "0", "0"],
            ["费率设置成功", "计费单位：60 分钟", "每单位收费：5.00 元"]
        )

        # 测试用例29: 查看当前费率
        self.run_test(
            "查看当前费率",
            ["999", "2", "0", "0"],
            ["当前费率", "计费单位", "每单位收费"]
        )

        # 创建测试卡片
        username = self.generate_random_username()
        password = self.generate_random_password()

        # 测试用例30: 删除卡
        self.run_test(
            f"删除卡 - {username}",
            ["1", username, password, "999", "3", username, "0", "0"],
            ["卡创建成功", "卡添加成功", "卡片删除成功"]
        )

        # 测试用例31: 验证删除的卡无法查询
        self.run_test(
            f"验证删除的卡无法查询 - {username}",
            ["2", username, "0"],
            ["未找到包含"]
        )

    def test_comprehensive_scenarios(self):
        """测试综合场景"""
        print("\n测试综合场景...")

        # 测试用例32-100: 各种综合场景
        for i in range(32, 101):
            username = self.generate_random_username()
            password = self.generate_random_password()

            # 随机选择一个综合场景
            scenario = random.choice([
                # 场景1: 完整流程
                {
                    'name': f"综合场景{i} - 完整流程 - {username}",
                    'inputs': ["1", username, password, "5", username, password, "100", 
                              "3", username, password, "4", username, password, 
                              "6", username, password, "50", "8", username, password, "0"],
                    'expected': ["卡创建成功", "卡添加成功", "充值成功", "上机成功", "下机成功", "退费成功", "注销卡成功"]
                },
                # 场景2: 多次充值
                {
                    'name': f"综合场景{i} - 多次充值 - {username}",
                    'inputs': ["1", username, password, "5", username, password, "50", 
                              "5", username, password, "30", "5", username, password, "20", "0"],
                    'expected': ["卡创建成功", "卡添加成功", "充值成功"]
                },
                # 场景3: 多次上下机
                {
                    'name': f"综合场景{i} - 多次上下机 - {username}",
                    'inputs': ["1", username, password, "5", username, password, "100", 
                              "3", username, password, "4", username, password,
                              "3", username, password, "4", username, password, "0"],
                    'expected': ["卡创建成功", "卡添加成功", "充值成功", "上机成功", "下机成功"]
                },
                # 场景4: 设置费率后上下机
                {
                    'name': f"综合场景{i} - 设置费率后上下机 - {username}",
                    'inputs': ["999", "1", "30", "3", "0", "1", username, password, 
                              "5", username, password, "100", "3", username, password, 
                              "4", username, password, "0"],
                    'expected': ["费率设置成功", "卡创建成功", "卡添加成功", "充值成功", "上机成功", "下机成功"]
                },
                # 场景5: 充值后查询余额
                {
                    'name': f"综合场景{i} - 充值后查询余额 - {username}",
                    'inputs': ["1", username, password, "5", username, password, "100", 
                              "2", username, password, "0"],
                    'expected': ["卡创建成功", "卡添加成功", "充值成功", "余额"]
                },
                # 场景6: 查询统计
                {
                    'name': f"综合场景{i} - 查询统计 - {username}",
                    'inputs': ["1", username, password, "5", username, password, "100", 
                              "3", username, password, "4", username, password, "7", "0"],
                    'expected': ["卡创建成功", "卡添加成功", "充值成功", "上机成功", "下机成功", "当日营收"]
                },
                # 场景7: 查看当前费率
                {
                    'name': f"综合场景{i} - 查看当前费率 - {username}",
                    'inputs': ["999", "2", "0", "0"],
                    'expected': ["当前费率", "计费单位", "每单位收费"]
                },
                # 场景8: 修改费率
                {
                    'name': f"综合场景{i} - 修改费率 - {username}",
                    'inputs': ["999", "1", "45", "4", "0", "999", "1", "60", "5", "0", "0"],
                    'expected': ["费率设置成功"]
                },
                # 场景9: 模糊查询
                {
                    'name': f"综合场景{i} - 模糊查询 - {username}",
                    'inputs': ["1", username, password, "2", username[:5], "1", password, "0"],
                    'expected': ["卡创建成功", "卡添加成功", "找到 1 个用户"]
                },
                # 场景10: 密码错误多次
                {
                    'name': f"综合场景{i} - 密码错误多次 - {username}",
                    'inputs': ["1", username, password, "2", username, "wrong1", 
                              username, "wrong2", username, "wrong3", "0"],
                    'expected': ["卡创建成功", "卡添加成功", "连续三次密码错误"]
                }
            ])

            self.run_test(
                scenario['name'],
                scenario['inputs'],
                scenario['expected']
            )

    def run_all_tests(self):
        """运行所有测试"""
        print("=" * 60)
        print("计费管理系统测试")
        print("=" * 60)
        print(f"开始时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print("=" * 60)

        # 运行各类测试
        self.test_add_card()
        self.test_query_card()
        self.test_logon()
        self.test_settle()
        self.test_add_money()
        self.test_refund_money()
        self.test_statistics()
        self.test_annual_card()
        self.test_hidden_menu()
        self.test_comprehensive_scenarios()

        # 输出测试结果
        print("\n" + "=" * 60)
        print("测试结果汇总")
        print("=" * 60)
        print(f"总测试数: {len(self.test_results)}")
        print(f"通过: {self.passed}")
        print(f"失败: {self.failed}")
        print(f"通过率: {self.passed / len(self.test_results) * 100:.2f}%")
        print(f"结束时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print("=" * 60)

        # 保存测试结果到文件
        self.save_test_results()

        return self.failed == 0

    def save_test_results(self):
        """保存测试结果到文件"""
        timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        filename = f"/home/dream/wut_workbench/tests/test_results_{timestamp}.txt"

        with open(filename, 'w', encoding='utf-8') as f:
            f.write("=" * 60 + "\n")
            f.write("计费管理系统测试结果\n")
            f.write("=" * 60 + "\n")
            f.write(f"测试时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
            f.write(f"总测试数: {len(self.test_results)}\n")
            f.write(f"通过: {self.passed}\n")
            f.write(f"失败: {self.failed}\n")
            f.write(f"通过率: {self.passed / len(self.test_results) * 100:.2f}%\n")
            f.write("=" * 60 + "\n\n")

            # 写入每个测试的详细结果
            for result in self.test_results:
                f.write(f"测试名称: {result['name']}\n")
                f.write(f"结果: {'通过' if result['passed'] else '失败'}\n")
                if not result['passed']:
                    f.write(f"错误: {result['error']}\n")
                    f.write(f"输出: {result['output'][:500]}\n")
                f.write("-" * 40 + "\n")

        print(f"\n测试结果已保存到: {filename}")

def main():
    # 获取项目根目录和可执行文件路径
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(script_dir)
    executable_path = os.path.join(project_root, "build", "main")

    # 检查可执行文件是否存在
    if not os.path.exists(executable_path):
        print(f"错误: 可执行文件 {executable_path} 不存在")
        print("请先编译项目: cd /home/dream/wut_workbench && make run")
        sys.exit(1)

    # 创建测试器并运行测试
    tester = BillingSystemTester(executable_path)
    success = tester.run_all_tests()

    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
