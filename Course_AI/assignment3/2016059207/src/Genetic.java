/*
 * 제가 구현한 유전 알고리즘입니다
 * 1. population size: 10
 * 2. parent selection method: tournament method => k는 3으로 두었습니다
 * 3. crossover rate: 80% => 20%만 부모 그대로 카피하고 , 나머지 자식들에 대해서는 one-point 크로스오버를 사용하였습니다
 * 4. mutation rate: 10% => 부모 포함 population 10개 중 1개에 대해서만 적용하였습니다
 * 5. fitness 함수: output이 낮아야 생존에 적합함을 의미합니다 => 가로,세로,대각선 공격 횟수로 정하였습니다
 * */


import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class Genetic {
	static int n = 0; // n-queens에서 n을 의미합니다
	static ArrayList<Integer> answer = null; // 과제명세서에 나온대로 각 col에서 queen이 배치된 row위치를 품고 있게 할 변수입니다
	static Individual[] curGeneration = null; // state와 이에 대한 fitness 값을 포함하는 개체(individual)들로 구성된 '현재 세대'입니다. state는 각 인덱스가 row 이고 각 칸에 들어가는 값이 col을 의미합니다
	static Individual[] nextGeneration = null; // 다음 세대입니다. 여기에 이전 세대에서 그대로 카피한 부모들, 크로스오버로 만든 자식들, mutation된 개체 등이 포함됩니다
	static Random random = new Random();
	static boolean findAns = false;
	static int cnt_generation = 0; // generation이 만들어진 횟수를 세기 위한 변수입니다
	
	public Genetic(int N) {
		n = N;
		answer = new ArrayList<>(n); 
		for(int i=0; i<n; i++) {
			answer.add(0);
		}
		curGeneration = new Individual[10];
		nextGeneration = new Individual[10];
		for(int i=0; i<10; i++) {
			curGeneration[i] = new Individual(n);
			nextGeneration[i] = new Individual(n);
		}
	}
	
	public void start() { // main에서 이 함수를 호출해야 유전 알고리즘이 실행됩니다
		random_individuals(); // 0번째 세대를 랜덤하게 생성합니다
		
		if(n==1) {
			findAns = true;
			for(int j=0; j<n; j++) {
				answer.set(0,1);
			}
			return;
		}
		if(n==2 || n==3) {
			return;
		}
		else {
			while(true) {
				genetic_process(); // 다음 세대를 생성하기 위해 호출합니다. 이 함수 내에서 parents selection, cross-over, mutation이 이루어집니다
				cnt_generation++;
				Individual bestIndividual = nextGeneration[0];
				for(int i=1; i<10; i++) {
					if(bestIndividual.fitness > nextGeneration[i].fitness) {
						bestIndividual = nextGeneration[i]; // 각 세대에서의 the fittest 개체를 구합니다
					}
				}
				
				//System.out.println(Arrays.toString(bestIndividual.curState));
				//System.out.print(" the fittest's score: ");
				//System.out.println(bestIndividual.fitness);
				
				if(bestIndividual.fitness == 0) { // 현재 탐색 중인 세대에서의 the fittest 개체가 정답이라면 답을 표시하고 리턴합니다
					findAns = true;
					for(int j=0; j<n; j++) {
						answer.set(bestIndividual.curState[j],j);
					}
					return;
				}
				else { // 현재 탐색 중인 세대에서의 the fittest 개체가 정답이 아니라면 필요한 메모리 할당을 다시 하고 while문을 다시 반복합니다
					curGeneration = nextGeneration;
					nextGeneration = new Individual[10];
					for(int i=0; i<10; i++) {
						nextGeneration[i] = new Individual(n);
					}
				}
			}
		}
	}
	
	public void genetic_process() { // 이 함수 리턴 시 다음 세대가 완성됩니다
		Individual[] num_k = new Individual[3]; // parent selection에서 사용하는 파라미터 k를 위한 변수입니다
		
		num_k[0] = curGeneration[random.nextInt(10)]; // 한 세대 내에서 랜덤하게 k개의 개체를 고릅니다
		Individual mother = num_k[0];
		for(int i=1; i<3; i++) {
			num_k[i] = curGeneration[random.nextInt(10)]; // with replacement를 허용합니다
			if(num_k[i].fitness < mother.fitness) mother = num_k[i]; // k개 중 가장 fitness한 개체를 다음 세대로 그대로 카피해서 보낼 부모 중 한명으로 선정합니다
		}
		
		num_k[0] = curGeneration[random.nextInt(10)]; // crossover rate을 20%로 잡았기 때문에 10개 중 부모로 2개 선택하려고 해당 코드를 반복하였습니다
		Individual father = num_k[0];
		for(int i=1; i<3; i++) {
			num_k[i] = curGeneration[random.nextInt(10)];
			if(num_k[i].fitness < father.fitness) father = num_k[i];
		}
		
		Individual newChild1 = new Individual(n); // 부모 개체들에 대한 메모리를 deep copy하여 다음 세대로 보낼 준비를 합니다 
		System.arraycopy(mother.curState, 0, newChild1.curState, 0, mother.curState.length);
		newChild1.fitness = mother.fitness;
		Individual newChild2 = new Individual(n);
		System.arraycopy(father.curState, 0, newChild2.curState, 0, father.curState.length);
		newChild2.fitness = father.fitness;
		
		nextGeneration[0] = newChild1; nextGeneration[1] = newChild2; // 부모들을 그대로 다음 세대로 보냅니다
		for(int i=2; i<10; i++) {
			Individual newChild = new Individual(n);
			System.arraycopy(mother.curState, 0, newChild.curState, 0, mother.curState.length); // 자식은 엄마 개체를 복사하여 물려받습니다(cross-over은 바로 아래)
			
			// cross-over 부분입니다
			int row_father = random.nextInt(n);
			int col_father = father.curState[row_father]; // 아빠 개체의 한 row(랜덤하게 선택)에 배치한 queen의 column 위치를 읽은 뒤, 
			newChild.curState[row_father] = col_father; // 자식에 대해 동일한 row에 배치한 queen의 column 위치를 수정토록 합니다. 즉, subspace에 대한 replace를 수행합니다
			newChild.fitness = fitness(newChild.curState); // state가 바뀌었으니 fitness 점수도 올바른 값을 가지도록 업데이트합니다
			
			nextGeneration[i] = newChild; // 이렇게 cross-over을 적용한 자식 개체를 다음 세대로 넣습니다
		}
		
		// mutation 부분
		int one_mutation = random.nextInt(10);
		nextGeneration[one_mutation].curState[random.nextInt(n)] = random.nextInt(n); // 한 세대에서 랜덤하게 고른 개체에 대해 mutation을 적용합니다
		nextGeneration[one_mutation].fitness = fitness(nextGeneration[one_mutation].curState); // mutation을 적용했으니 fitness 점수도 업데이트 합니다.
	}
	
	public void random_individuals() { //  0번째 세대를 랜덤하게 만들기 위한 함수입니다
		for(int i=0; i<10; i++) {
		for(int j=0; j<n; j++) {
			curGeneration[i].curState[j] = random.nextInt(n); // 퀸의 배치를 랜덤하게 해서 개별 개체를 만듭니다
		}
		int fitV = fitness(curGeneration[i].curState); 
		curGeneration[i].fitness = fitV; // 각 개체별 fitness 점수를 기록합니다
		}
		
		Individual bestIndividual = curGeneration[0];
		for(int i=1; i<10; i++) {
			if(bestIndividual.fitness > curGeneration[i].fitness) {
				bestIndividual = curGeneration[i]; // 0번째 세대에서의 the fittest 개체를 구합니다
			}
		}
		
		if(bestIndividual.fitness == 0) { // 0번쨰 세대에서의 the fittest 개체가 정답이라면 답을 표시하고 리턴합니다
			findAns = true;
			for(int j=0; j<n; j++) {
				answer.set(bestIndividual.curState[j],j);
			}
			return;
		}
	}
	
	public int fitness(int[] state) { // fitness 함수입니다. 공격 횟수가 많을수록 output값이 커집니다 즉, 결과가 작아야 좋은 fitness를 의미합니다
	    // col[1] = 1행 *열
	    // col[1] = 1 => 1행 1열, col[2] = 3 => 2행 3열
		int[] col = state;
		int num_attack = 0; // 퀸 끼리 공격을 많이 할수록 증가하는 값입니다
	    
		for(int c = 1; c<n; c++) {
	    for (int i = 0; i < c; i++) {
	        // 같은 행, 열에 대해 체크합니다
	        if (col[i] == col[c]) {
	        	num_attack+=1;
	        }
	        // 대각선에 대해 체크합니다
	        if (Math.abs(col[i] - col[c]) == Math.abs(i - c)) {
	        	num_attack+=1;
	        }
	    }
		}
		return num_attack; // 실제 정답이라면 0을 반환할 것입니다
	}
	
	public String make_result() { // 결과에 해당하는 문자열을 출력할 함수입니다
		String str = "Location : ";
		for(int i : answer) {
			String s = Integer.toString(i);
			str += (s + " ");
		}
		str += '\n';
		return str;
	}
	
}
